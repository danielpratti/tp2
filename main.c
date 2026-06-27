#include <stdio.h>
#include <string.h>
#include "bdtimes.h"
#include "bdpartidas.h"

//Limpa o buffer do stdin após leitura
static void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

//Funcionalidade 1: Consultar time
static void menu_consultar_time(BDTimes *bdt) {
    char prefixo[MAX_NOME];
    printf("\nDigite o nome ou prefixo do time: ");
    scanf("%49s", prefixo);
    limpar_buffer();

    Time *resultados[MAX_TIMES];
    int n = bdtimes_buscar_por_prefixo(bdt, prefixo, resultados, MAX_TIMES);

    if (n == 0) {
        printf("Nenhum time encontrado com o prefixo '%s'.\n\n", prefixo);
        return;
    }

    printf("\n");
    time_imprimir_cabecalho();
    for (int i = 0; i < n; i++)
        time_imprimir(resultados[i]);
    printf("\n");
}

//Funcionalidade 2: Consultar partidas
static void menu_consultar_partidas(BDPartidas *bdp, BDTimes *bdt) {
    printf("\nEscolha o modo de consulta:\n");
    printf("  1 - Por time mandante\n");
    printf("  2 - Por time visitante\n");
    printf("  3 - Por time mandante ou visitante\n");
    printf("  4 - Retornar ao menu principal\n");
    printf("Opcao: ");

    int modo;
    scanf("%d", &modo);
    limpar_buffer();

    if (modo == 4) return;
    if (modo < 1 || modo > 3) {
        printf("Opcao invalida.\n\n");
        return;
    }

    char prefixo[MAX_NOME];
    printf("Digite o nome ou prefixo do time: ");
    scanf("%49s", prefixo);
    limpar_buffer();

    bdpartidas_consultar(bdp, bdt, prefixo, modo);
}

//Menu principal
int main(void) {
    BDTimes  bdt;
    BDPartidas bdp;

    bdtimes_init(&bdt);
    bdpartidas_init(&bdp);

    //Carrega os arquivos CSV
    if (bdtimes_carregar(&bdt, "times.csv") != 0) {
        fprintf(stderr, "Erro fatal: nao foi possivel carregar times.csv\n");
        return 1;
    }
    if (bdpartidas_carregar(&bdp, &bdt, "partidas.csv") != 0) {
        fprintf(stderr, "Erro fatal: nao foi possivel carregar partidas.csv\n");
        return 1;
    }

    printf("=== Sistema de Gerenciamento de Partidas ===\n\n");

    char opcao[4];
    do {
        printf("1 - Consultar time\n");
        printf("2 - Consultar partidas\n");
        printf("3 - Atualizar partida  [desabilitado]\n");
        printf("4 - Remover partida    [desabilitado]\n");
        printf("5 - Inserir partida    [desabilitado]\n");
        printf("6 - Imprimir tabela de classificacao\n");
        printf("Q - Sair\n");
        printf("Opcao: ");

        scanf("%3s", opcao);
        limpar_buffer();

        if (strcmp(opcao, "1") == 0) {
            menu_consultar_time(&bdt);
        } else if (strcmp(opcao, "2") == 0) {
            menu_consultar_partidas(&bdp, &bdt);
        } else if (strcmp(opcao, "3") == 0 || strcmp(opcao, "4") == 0 || strcmp(opcao, "5") == 0) {
            printf("\nFuncionalidade nao implementada nesta versao.\n\n");
        } else if (strcmp(opcao, "6") == 0) {
            bdtimes_imprimir_tabela(&bdt);
        } else if (strcmp(opcao, "Q") == 0 || strcmp(opcao, "q") == 0) {
            printf("\nEncerrando o sistema. :D\n");
        } else {
            printf("\nOpcao invalida. Tente novamente.\n\n");
        }

    } while (strcmp(opcao, "Q") != 0 && strcmp(opcao, "q") != 0);

    return 0;
}
