#include "bdpartidas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void bdpartidas_inicializar(BDPartidas *bd) {
    bd->inicio = NULL;
    bd->total = 0;
    bd->proximo_id = 0;
}

void bdpartidas_destruir(BDPartidas *bd) {
    NoPartida *atual = bd->inicio;
    while (atual != NULL) {
        NoPartida *prox = atual->prox;
        free(atual);
        atual = prox;
    }
    bd->inicio = NULL;
    bd->total = 0;
}

/* Insere um no ja construido ao final da lista (funcao auxiliar interna). */
static int inserir_no_final(BDPartidas *bd, Partida p) {
    NoPartida *novo = (NoPartida *) malloc(sizeof(NoPartida));
    if (novo == NULL) {
        return 0;
    }
    novo->dado = p;
    novo->prox = NULL;

    if (bd->inicio == NULL) {
        bd->inicio = novo;
    } else {
        NoPartida *atual = bd->inicio;
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = novo;
    }
    bd->total++;
    return 1;
}

int bdpartidas_inserir(BDPartidas *bd, int id_time1, int id_time2, int gols_time1, int gols_time2) {
    int novo_id = bd->proximo_id;
    Partida p = partida_criar(novo_id, id_time1, id_time2, gols_time1, gols_time2);

    if (!inserir_no_final(bd, p)) {
        return -1;
    }
    bd->proximo_id++;
    return novo_id;
}

Partida *bdpartidas_buscar_por_id(BDPartidas *bd, int id) {
    NoPartida *atual = bd->inicio;
    while (atual != NULL) {
        if (atual->dado.id == id) {
            return &(atual->dado);
        }
        atual = atual->prox;
    }
    return NULL;
}

/* Compara, sem diferenciar maiusculas/minusculas, se `nome` comeca com `prefixo`. */
static int comeca_com_prefixo(const char *nome, const char *prefixo) {
    size_t i;
    size_t tam_prefixo = strlen(prefixo);

    for (i = 0; i < tam_prefixo; i++) {
        if (nome[i] == '\0') {
            return 0;
        }
        if (tolower((unsigned char) nome[i]) != tolower((unsigned char) prefixo[i])) {
            return 0;
        }
    }
    return 1;
}

int bdpartidas_buscar_por_time(BDPartidas *bd, BDTimes *bdtimes, const char *prefixo,
                                ModoConsultaPartida modo, Partida *resultado[], int capacidade) {
    int qtd = 0;
    NoPartida *atual = bd->inicio;

    while (atual != NULL && qtd < capacidade) {
        Time *t1 = bdtimes_buscar_por_id(bdtimes, atual->dado.id_time1);
        Time *t2 = bdtimes_buscar_por_id(bdtimes, atual->dado.id_time2);

        int casa_mandante = (t1 != NULL) && comeca_com_prefixo(t1->nome, prefixo);
        int casa_visitante = (t2 != NULL) && comeca_com_prefixo(t2->nome, prefixo);

        int corresponde = 0;
        if (modo == MODO_MANDANTE && casa_mandante) {
            corresponde = 1;
        } else if (modo == MODO_VISITANTE && casa_visitante) {
            corresponde = 1;
        } else if (modo == MODO_AMBOS && (casa_mandante || casa_visitante)) {
            corresponde = 1;
        }

        if (corresponde) {
            resultado[qtd] = &(atual->dado);
            qtd++;
        }
        atual = atual->prox;
    }
    return qtd;
}

int bdpartidas_atualizar(BDPartidas *bd, int id, int novo_gols_time1, int novo_gols_time2) {
    Partida *p = bdpartidas_buscar_por_id(bd, id);
    if (p == NULL) {
        return 0;
    }

    /* Sentinela -1 indica "manter o valor atual" do campo. */
    if (novo_gols_time1 >= 0) {
        p->gols_time1 = novo_gols_time1;
    }
    if (novo_gols_time2 >= 0) {
        p->gols_time2 = novo_gols_time2;
    }
    return 1;
}

int bdpartidas_remover(BDPartidas *bd, int id) {
    NoPartida *atual = bd->inicio;
    NoPartida *anterior = NULL;

    while (atual != NULL) {
        if (atual->dado.id == id) {
            if (anterior == NULL) {
                bd->inicio = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }
            free(atual);
            bd->total--;
            return 1;
        }
        anterior = atual;
        atual = atual->prox;
    }
    return 0;
}

int bdpartidas_total(const BDPartidas *bd) {
    return bd->total;
}

int bdpartidas_para_vetor(BDPartidas *bd, Partida *vetor[], int capacidade) {
    int qtd = 0;
    NoPartida *atual = bd->inicio;

    while (atual != NULL && qtd < capacidade) {
        vetor[qtd] = &(atual->dado);
        qtd++;
        atual = atual->prox;
    }
    return qtd;
}

int bdpartidas_carregar_csv(BDPartidas *bd, const char *caminho_arquivo) {
    FILE *arquivo = fopen(caminho_arquivo, "r");
    if (arquivo == NULL) {
        return -1;
    }

    char linha[256];
    int qtd_carregados = 0;
    int maior_id = -1;

    /* Descarta a linha de cabecalho. */
    if (fgets(linha, sizeof(linha), arquivo) == NULL) {
        fclose(arquivo);
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';

        if (strlen(linha) == 0) {
            continue;
        }

        char *token_id = strtok(linha, ",");
        char *token_t1 = strtok(NULL, ",");
        char *token_t2 = strtok(NULL, ",");
        char *token_g1 = strtok(NULL, ",");
        char *token_g2 = strtok(NULL, ",");

        if (token_id == NULL || token_t1 == NULL || token_t2 == NULL ||
            token_g1 == NULL || token_g2 == NULL) {
            continue;
        }

        int id = atoi(token_id);
        int t1 = atoi(token_t1);
        int t2 = atoi(token_t2);
        int g1 = atoi(token_g1);
        int g2 = atoi(token_g2);

        Partida p = partida_criar(id, t1, t2, g1, g2);
        if (inserir_no_final(bd, p)) {
            qtd_carregados++;
            if (id > maior_id) {
                maior_id = id;
            }
        }
    }

    fclose(arquivo);
    bd->proximo_id = maior_id + 1;
    return qtd_carregados;
}

int bdpartidas_salvar_csv(const BDPartidas *bd, const char *caminho_arquivo) {
    FILE *arquivo = fopen(caminho_arquivo, "w");
    if (arquivo == NULL) {
        return 0;
    }

    fprintf(arquivo, "ID,Time1,Time2,GolsTime1,GolsTime2\n");

    NoPartida *atual = bd->inicio;
    while (atual != NULL) {
        fprintf(arquivo, "%d,%d,%d,%d,%d\n",
                atual->dado.id, atual->dado.id_time1, atual->dado.id_time2,
                atual->dado.gols_time1, atual->dado.gols_time2);
        atual = atual->prox;
    }

    fclose(arquivo);
    return 1;
}
