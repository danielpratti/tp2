#include "ui.h"
#include "partidas_servico.h"
#include "ordenacao.h"
#include "entrada.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RESULTADO 600
#define LINHAS_POR_PAGINA 5

/* ---------------------------------------------------------------- */
/* Funcoes auxiliares internas                                      */
/* ---------------------------------------------------------------- */

/* Retorna o nome do time com o ID informado, ou "Desconhecido" se nao encontrado. */
static const char *nome_do_time(BDTimes *bdtimes, int id) {
    Time *t = bdtimes_buscar_por_id(bdtimes, id);
    return (t != NULL) ? t->nome : "Desconhecido";
}

/* Imprime o cabecalho da tabela usada nas consultas/listagens de partidas. */
static void imprimir_cabecalho_partidas(void) {
    printf("%-4s %-12s %-12s %7s %7s\n", "ID", "Time1", "Time2", "Placar1", "Placar2");
    printf("--------------------------------------------------------\n");
}

/* Imprime uma linha de partida, resolvendo os nomes dos times pelos IDs. */
static void imprimir_linha_partida(BDTimes *bdtimes, const Partida *p) {
    printf("%-4d %-12s %-12s %7d %7d\n",
           p->id, nome_do_time(bdtimes, p->id_time1), nome_do_time(bdtimes, p->id_time2),
           p->gols_time1, p->gols_time2);
}

/*
 * Executa o fluxo de consulta de partidas (escolha de modo + prefixo),
 * usado tanto pela funcionalidade de consulta isolada quanto como
 * primeiro passo das funcionalidades de atualizacao/remocao. Preenche
 * `resultado` com os ponteiros encontrados e retorna a quantidade.
 */
static int fluxo_consulta_partidas(BDPartidas *bdpartidas, BDTimes *bdtimes, Partida *resultado[]) {
    printf("\nEscolha o modo de consulta:\n");
    printf("1 - Por time mandante\n");
    printf("2 - Por time visitante\n");
    printf("3 - Por time mandante ou visitante\n");
    printf("4 - Retornar ao menu principal\n");

    int opcao = entrada_ler_inteiro("Opcao: ", 0);
    if (opcao == -2) {
        return -1; /* EOF: aborta o fluxo como se tivesse retornado ao menu. */
    }
    if (opcao == 4) {
        return -1;
    }
    if (opcao < 1 || opcao > 3) {
        printf("Opcao invalida.\n");
        return 0;
    }

    char prefixo[MAX_NOME];
    printf("Digite o nome ou prefixo do time: ");
    if (!entrada_ler_linha(prefixo, sizeof(prefixo))) {
        return -1; /* EOF */
    }

    ModoConsultaPartida modo = (ModoConsultaPartida) opcao;
    int qtd = bdpartidas_buscar_por_time(bdpartidas, bdtimes, prefixo, modo, resultado, MAX_RESULTADO);

    if (qtd == 0) {
        printf("\nNenhuma partida encontrada para o criterio informado.\n");
        return 0;
    }

    printf("\n");
    imprimir_cabecalho_partidas();
    for (int i = 0; i < qtd; i++) {
        imprimir_linha_partida(bdtimes, resultado[i]);
    }
    return qtd;
}

/* ---------------------------------------------------------------- */
/* Menu principal                                                    */
/* ---------------------------------------------------------------- */

char ui_exibir_menu(void) {
    printf("\n==============================================\n");
    printf(" Sistema de Gerenciamento de Partidas\n");
    printf("==============================================\n");
    printf("1 - Consultar time\n");
    printf("2 - Consultar partidas\n");
    printf("3 - Atualizar partida\n");
    printf("4 - Remover partida\n");
    printf("5 - Inserir partida\n");
    printf("6 - Imprimir tabela de classificacao\n");
    printf("Q - Sair\n");
    printf("Escolha uma opcao: ");

    char linha[16];
    if (!entrada_ler_linha(linha, sizeof(linha))) {
        return 'Q'; /* EOF no stdin: encerra o sistema com seguranca. */
    }

    if (strlen(linha) == 0) {
        return '\0';
    }
    char opcao = linha[0];
    if (opcao >= 'a' && opcao <= 'z') {
        opcao = (char) (opcao - 'a' + 'A');
    }
    return opcao;
}

/* ---------------------------------------------------------------- */
/* Funcionalidade 1: Consultar time                                  */
/* ---------------------------------------------------------------- */

void ui_consultar_time(BDTimes *bdtimes) {
    char prefixo[MAX_NOME];
    printf("\nDigite o nome ou prefixo do time: ");
    if (!entrada_ler_linha(prefixo, sizeof(prefixo))) {
        return; /* EOF */
    }

    int capacidade = bdtimes_total(bdtimes);
    if (capacidade <= 0) {
        printf("Nenhum time cadastrado.\n");
        return;
    }

    Time **resultado = (Time **) malloc((size_t) capacidade * sizeof(Time *));
    if (resultado == NULL) {
        printf("Erro de alocacao de memoria.\n");
        return;
    }

    int qtd = bdtimes_buscar_por_prefixo(bdtimes, prefixo, resultado, capacidade);

    if (qtd == 0) {
        printf("\nNenhum time encontrado com o nome ou prefixo \"%s\".\n", prefixo);
    } else {
        printf("\n");
        time_imprimir_cabecalho();
        for (int i = 0; i < qtd; i++) {
            time_imprimir(resultado[i]);
        }
    }

    free(resultado);
}

/* ---------------------------------------------------------------- */
/* Funcionalidade 2: Consultar partidas                              */
/* ---------------------------------------------------------------- */

void ui_consultar_partidas(BDPartidas *bdpartidas, BDTimes *bdtimes) {
    Partida **resultado = (Partida **) malloc(MAX_RESULTADO * sizeof(Partida *));
    if (resultado == NULL) {
        printf("Erro de alocacao de memoria.\n");
        return;
    }

    fluxo_consulta_partidas(bdpartidas, bdtimes, resultado);

    free(resultado);
}

/* ---------------------------------------------------------------- */
/* Funcionalidade 3: Atualizar partida                               */
/* ---------------------------------------------------------------- */

void ui_atualizar_partida(BDPartidas *bdpartidas, BDTimes *bdtimes) {
    printf("\nPara atualizar uma partida, primeiro localize-a:\n");

    Partida **resultado = (Partida **) malloc(MAX_RESULTADO * sizeof(Partida *));
    if (resultado == NULL) {
        printf("Erro de alocacao de memoria.\n");
        return;
    }

    int qtd = fluxo_consulta_partidas(bdpartidas, bdtimes, resultado);
    if (qtd <= 0) {
        free(resultado);
        return;
    }

    int id = entrada_ler_inteiro("\nDigite o ID do registro a ser atualizado: ", 0);
    if (id == -2) {
        free(resultado);
        return; /* EOF */
    }

    Partida *p = bdpartidas_buscar_por_id(bdpartidas, id);
    if (p == NULL) {
        printf("Registro com ID %d nao encontrado entre os resultados.\n", id);
        free(resultado);
        return;
    }

    printf("\nDigite o novo valor para os campos Placar1 e Placar2\n");
    printf("(para manter o valor atual de um campo, digite '-'):\n");
    int novo_g1 = entrada_ler_inteiro("Placar1: ", 1);
    int novo_g2 = entrada_ler_inteiro("Placar2: ", 1);

    if (novo_g1 == -2 || novo_g2 == -2) {
        free(resultado);
        return; /* EOF */
    }

    int exibir_g1 = (novo_g1 >= 0) ? novo_g1 : p->gols_time1;
    int exibir_g2 = (novo_g2 >= 0) ? novo_g2 : p->gols_time2;

    printf("\nConfirma os novos valores para o registro abaixo? (S/N)\n");
    imprimir_cabecalho_partidas();
    printf("%-4d %-12s %-12s %7d %7d\n",
           p->id, nome_do_time(bdtimes, p->id_time1), nome_do_time(bdtimes, p->id_time2),
           exibir_g1, exibir_g2);

    if (entrada_ler_confirmacao("Confirmar? (S/N): ")) {
        servico_atualizar_partida(bdpartidas, bdtimes, id, novo_g1, novo_g2);
        printf("\nRegistro atualizado com sucesso.\n");
    } else {
        printf("\nAtualizacao cancelada.\n");
    }

    free(resultado);
}

/* ---------------------------------------------------------------- */
/* Funcionalidade 4: Remover partida                                 */
/* ---------------------------------------------------------------- */

void ui_remover_partida(BDPartidas *bdpartidas, BDTimes *bdtimes) {
    printf("\nPara remover uma partida, primeiro localize-a:\n");

    Partida **resultado = (Partida **) malloc(MAX_RESULTADO * sizeof(Partida *));
    if (resultado == NULL) {
        printf("Erro de alocacao de memoria.\n");
        return;
    }

    int qtd = fluxo_consulta_partidas(bdpartidas, bdtimes, resultado);
    if (qtd <= 0) {
        free(resultado);
        return;
    }

    int id = entrada_ler_inteiro("\nDigite o ID do registro a ser removido: ", 0);
    if (id == -2) {
        free(resultado);
        return; /* EOF */
    }

    Partida *p = bdpartidas_buscar_por_id(bdpartidas, id);
    if (p == NULL) {
        printf("Registro com ID %d nao encontrado entre os resultados.\n", id);
        free(resultado);
        return;
    }

    printf("\nTem certeza de que deseja excluir o registro abaixo? (S/N)\n");
    imprimir_cabecalho_partidas();
    imprimir_linha_partida(bdtimes, p);

    if (entrada_ler_confirmacao("Confirmar? (S/N): ")) {
        servico_remover_partida(bdpartidas, bdtimes, id);
        printf("\nRegistro removido com sucesso.\n");
    } else {
        printf("\nRemocao cancelada.\n");
    }

    free(resultado);
}

/* ---------------------------------------------------------------- */
/* Funcionalidade 5: Inserir partida                                 */
/* ---------------------------------------------------------------- */

void ui_inserir_partida(BDPartidas *bdpartidas, BDTimes *bdtimes) {
    printf("\nPara inserir um novo registro, digite os valores para\n");
    printf("os campos Time1, Time2, Placar1 e Placar2:\n");

    int id_time1 = entrada_ler_inteiro("ID do Time1: ", 0);
    int id_time2 = entrada_ler_inteiro("ID do Time2: ", 0);
    if (id_time1 == -2 || id_time2 == -2) {
        return; /* EOF */
    }

    /* Validacao: ambos os times precisam existir no BDTimes. */
    Time *t1 = bdtimes_buscar_por_id(bdtimes, id_time1);
    if (t1 == NULL) {
        printf("\nErro: nao existe time cadastrado com ID %d.\n", id_time1);
        return;
    }
    Time *t2 = bdtimes_buscar_por_id(bdtimes, id_time2);
    if (t2 == NULL) {
        printf("\nErro: nao existe time cadastrado com ID %d.\n", id_time2);
        return;
    }
    if (id_time1 == id_time2) {
        printf("\nErro: Time1 e Time2 nao podem ser o mesmo time.\n");
        return;
    }

    int placar1 = entrada_ler_inteiro("Placar1: ", 0);
    int placar2 = entrada_ler_inteiro("Placar2: ", 0);

    if (placar1 == -2 || placar2 == -2) {
        return; /* EOF */
    }

    if (placar1 < 0 || placar2 < 0) {
        printf("\nErro: o placar nao pode ser negativo.\n");
        return;
    }

    printf("\nConfirma a insercao do registro abaixo? (S/N)\n");
    imprimir_cabecalho_partidas();
    printf("%-4s %-12s %-12s %7d %7d\n", "?", t1->nome, t2->nome, placar1, placar2);

    if (!entrada_ler_confirmacao("Confirmar? (S/N): ")) {
        printf("\nInsercao cancelada.\n");
        return;
    }

    int novo_id = servico_inserir_partida(bdpartidas, bdtimes, id_time1, id_time2, placar1, placar2);
    if (novo_id < 0) {
        printf("\nErro ao inserir o registro (falha de alocacao de memoria).\n");
        return;
    }

    printf("\nO registro foi inserido com sucesso.\n");
    printf("%-4s %-12s %-12s %7s %7s\n", "ID", "Time1", "Time2", "Placar1", "Placar2");
    printf("%-4d %-12s %-12s %7d %7d\n", novo_id, t1->nome, t2->nome, placar1, placar2);
}

/* ---------------------------------------------------------------- */
/* Funcionalidade 6: Imprimir tabela de classificacao                */
/* ---------------------------------------------------------------- */

void ui_imprimir_classificacao(BDTimes *bdtimes) {
    int total = bdtimes_total(bdtimes);
    if (total <= 0) {
        printf("\nNenhum time cadastrado.\n");
        return;
    }

    Time **vetor = (Time **) malloc((size_t) total * sizeof(Time *));
    if (vetor == NULL) {
        printf("Erro de alocacao de memoria.\n");
        return;
    }
    bdtimes_para_vetor(bdtimes, vetor, total);

    ordenar_classificacao(vetor, total);

    printf("\nImprimindo classificacao...\n\n");

    int pagina = 0;
    int total_paginas = (total + LINHAS_POR_PAGINA - 1) / LINHAS_POR_PAGINA;

    while (pagina < total_paginas) {
        time_imprimir_cabecalho();

        int inicio = pagina * LINHAS_POR_PAGINA;
        int fim = inicio + LINHAS_POR_PAGINA;
        if (fim > total) {
            fim = total;
        }

        for (int i = inicio; i < fim; i++) {
            time_imprimir(vetor[i]);
        }

        pagina++;

        if (pagina < total_paginas) {
            printf("\n-- Pagina %d de %d. Pressione ENTER para continuar (Q para sair) --\n",
                   pagina, total_paginas);
            char linha[16];
            if (!entrada_ler_linha(linha, sizeof(linha))) {
                break; /* EOF: interrompe a paginacao com seguranca. */
            }
            if (strlen(linha) > 0 && (linha[0] == 'q' || linha[0] == 'Q')) {
                break;
            }
            printf("\n");
        }
    }

    free(vetor);
}
