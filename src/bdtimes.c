#include "bdtimes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void bdtimes_inicializar(BDTimes *bd) {
    bd->inicio = NULL;
    bd->total = 0;
}

void bdtimes_destruir(BDTimes *bd) {
    NoTime *atual = bd->inicio;
    while (atual != NULL) {
        NoTime *prox = atual->prox;
        free(atual);
        atual = prox;
    }
    bd->inicio = NULL;
    bd->total = 0;
}

int bdtimes_inserir(BDTimes *bd, Time t) {
    NoTime *novo = (NoTime *) malloc(sizeof(NoTime));
    if (novo == NULL) {
        return 0;
    }
    novo->dado = t;
    novo->prox = NULL;

    /* Insere ao final, preservando a ordem de carga/cadastro. */
    if (bd->inicio == NULL) {
        bd->inicio = novo;
    } else {
        NoTime *atual = bd->inicio;
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = novo;
    }
    bd->total++;
    return 1;
}

Time *bdtimes_buscar_por_id(BDTimes *bd, int id) {
    NoTime *atual = bd->inicio;
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

int bdtimes_buscar_por_prefixo(BDTimes *bd, const char *prefixo, Time *resultado[], int capacidade) {
    int qtd = 0;
    NoTime *atual = bd->inicio;

    while (atual != NULL && qtd < capacidade) {
        if (comeca_com_prefixo(atual->dado.nome, prefixo)) {
            resultado[qtd] = &(atual->dado);
            qtd++;
        }
        atual = atual->prox;
    }
    return qtd;
}

int bdtimes_total(const BDTimes *bd) {
    return bd->total;
}

int bdtimes_para_vetor(BDTimes *bd, Time *vetor[], int capacidade) {
    int qtd = 0;
    NoTime *atual = bd->inicio;

    while (atual != NULL && qtd < capacidade) {
        vetor[qtd] = &(atual->dado);
        qtd++;
        atual = atual->prox;
    }
    return qtd;
}

int bdtimes_carregar_csv(BDTimes *bd, const char *caminho_arquivo) {
    FILE *arquivo = fopen(caminho_arquivo, "r");
    if (arquivo == NULL) {
        return -1;
    }

    char linha[256];
    int qtd_carregados = 0;

    /* Descarta a linha de cabecalho (ID,Nome). */
    if (fgets(linha, sizeof(linha), arquivo) == NULL) {
        fclose(arquivo);
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        /* Remove a quebra de linha final, se houver. */
        linha[strcspn(linha, "\r\n")] = '\0';

        if (strlen(linha) == 0) {
            continue;
        }

        int id;
        char nome[MAX_NOME];

        char *token_id = strtok(linha, ",");
        char *token_nome = strtok(NULL, ",");

        if (token_id == NULL || token_nome == NULL) {
            continue;
        }

        id = atoi(token_id);
        strncpy(nome, token_nome, MAX_NOME - 1);
        nome[MAX_NOME - 1] = '\0';

        Time t = time_criar(id, nome);
        if (bdtimes_inserir(bd, t)) {
            qtd_carregados++;
        }
    }

    fclose(arquivo);
    return qtd_carregados;
}

int bdtimes_salvar_csv(const BDTimes *bd, const char *caminho_arquivo) {
    FILE *arquivo = fopen(caminho_arquivo, "w");
    if (arquivo == NULL) {
        return 0;
    }

    fprintf(arquivo, "ID,Nome\n");

    NoTime *atual = bd->inicio;
    while (atual != NULL) {
        fprintf(arquivo, "%d,%s\n", atual->dado.id, atual->dado.nome);
        atual = atual->prox;
    }

    fclose(arquivo);
    return 1;
}
