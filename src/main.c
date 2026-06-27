#include <stdio.h>
#include <stdlib.h>
#include "bdtimes.h"
#include "bdpartidas.h"
#include "partidas_servico.h"
#include "ui.h"

/*
 * Arquivos de entrada (carga inicial) e de saida (persistencia).
 *
 * O enunciado pede que sejam entregues bd_partida.csv e
 * bd_classificacao.csv. Adotamos a seguinte convencao:
 *   - Entrada: times.csv / partidas.csv (espelham o estado inicial)
 *   - Saida:   bd_partida.csv (snapshot das partidas apos a sessao)
 *              bd_classificacao.csv (tabela de classificacao final)
 */
#define ARQUIVO_TIMES_ENTRADA "times.csv"
#define ARQUIVO_PARTIDAS_ENTRADA "partidas.csv"
#define ARQUIVO_PARTIDAS_SAIDA "bd_partida.csv"
#define ARQUIVO_CLASSIFICACAO_SAIDA "bd_classificacao.csv"

/* Salva a tabela de classificacao (ja com as estatisticas atuais) em CSV. */
static void salvar_classificacao_csv(BDTimes *bdtimes, const char *caminho) {
    int total = bdtimes_total(bdtimes);
    if (total <= 0) {
        return;
    }

    FILE *arquivo = fopen(caminho, "w");
    if (arquivo == NULL) {
        printf("Aviso: nao foi possivel salvar %s.\n", caminho);
        return;
    }

    Time **vetor = (Time **) malloc((size_t) total * sizeof(Time *));
    if (vetor == NULL) {
        fclose(arquivo);
        return;
    }
    bdtimes_para_vetor(bdtimes, vetor, total);

    fprintf(arquivo, "ID,Time,V,E,D,GM,GS,S,PG\n");
    for (int i = 0; i < total; i++) {
        Time *t = vetor[i];
        fprintf(arquivo, "%d,%s,%d,%d,%d,%d,%d,%d,%d\n",
                t->id, t->nome, t->vitorias, t->empates, t->derrotas,
                t->gols_marcados, t->gols_sofridos, time_saldo(t), time_pontos(t));
    }

    free(vetor);
    fclose(arquivo);
}

int main(void) {
    BDTimes bdtimes;
    BDPartidas bdpartidas;

    bdtimes_inicializar(&bdtimes);
    bdpartidas_inicializar(&bdpartidas);

    /* Carrega o cadastro de times. */
    int qtd_times = bdtimes_carregar_csv(&bdtimes, ARQUIVO_TIMES_ENTRADA);
    if (qtd_times < 0) {
        fprintf(stderr, "Erro: nao foi possivel abrir o arquivo '%s'.\n", ARQUIVO_TIMES_ENTRADA);
        return 1;
    }

    /* Carrega o historico de partidas. */
    int qtd_partidas = bdpartidas_carregar_csv(&bdpartidas, ARQUIVO_PARTIDAS_ENTRADA);
    if (qtd_partidas < 0) {
        fprintf(stderr, "Erro: nao foi possivel abrir o arquivo '%s'.\n", ARQUIVO_PARTIDAS_ENTRADA);
        bdtimes_destruir(&bdtimes);
        return 1;
    }

    /* Calcula, a partir do historico carregado, as estatisticas iniciais dos times. */
    servico_recalcular_todas_estatisticas(&bdpartidas, &bdtimes);

    printf("Sistema iniciado: %d times e %d partidas carregados.\n", qtd_times, qtd_partidas);

    /* Laco principal do menu. */
    char opcao;
    do {
        opcao = ui_exibir_menu();

        switch (opcao) {
            case '1':
                ui_consultar_time(&bdtimes);
                break;
            case '2':
                ui_consultar_partidas(&bdpartidas, &bdtimes);
                break;
            case '3':
                ui_atualizar_partida(&bdpartidas, &bdtimes);
                break;
            case '4':
                ui_remover_partida(&bdpartidas, &bdtimes);
                break;
            case '5':
                ui_inserir_partida(&bdpartidas, &bdtimes);
                break;
            case '6':
                ui_imprimir_classificacao(&bdtimes);
                break;
            case 'Q':
                printf("\nSalvando dados e encerrando o sistema...\n");
                break;
            default:
                printf("\nOpcao invalida. Tente novamente.\n");
                break;
        }
    } while (opcao != 'Q');

    /* Persistencia final: grava o estado atualizado das partidas e da classificacao. */
    bdpartidas_salvar_csv(&bdpartidas, ARQUIVO_PARTIDAS_SAIDA);
    salvar_classificacao_csv(&bdtimes, ARQUIVO_CLASSIFICACAO_SAIDA);

    printf("Dados salvos em '%s' e '%s'.\n", ARQUIVO_PARTIDAS_SAIDA, ARQUIVO_CLASSIFICACAO_SAIDA);

    bdpartidas_destruir(&bdpartidas);
    bdtimes_destruir(&bdtimes);

    return 0;
}
