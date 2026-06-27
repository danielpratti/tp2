#include "bdpartidas.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>

//Inicializa BD de partidas com quantidade zero
void bdpartidas_init(BDPartidas *bd) {
    bd->quantidade = 0;
}

/*
 * Carrega partidas do CSV (ID,Time1,Time2,GolsTime1,GolsTime2).
 * Para cada partida lida, atualiza as estatísticas dos times no BDTimes.
 */
int bdpartidas_carregar(BDPartidas *bd, BDTimes *bdt, const char *arquivo) {
    FILE *f = fopen(arquivo, "r");
    if (!f) {
        fprintf(stderr, "Erro: nao foi possivel abrir '%s'\n", arquivo);
        return -1;
    }

    char linha[256];
    //Pula cabeçalho
    fgets(linha, sizeof(linha), f);

    while (fgets(linha, sizeof(linha), f) && bd->quantidade < MAX_PARTIDAS) {
        int id, t1, t2, g1, g2;
        if (sscanf(linha, "%d,%d,%d,%d,%d", &id, &t1, &t2, &g1, &g2) == 5) {
            bd->partidas[bd->quantidade] = partida_criar(id, t1, t2, g1, g2);
            bd->quantidade++;

            //Atualiza estatísticas dos times envolvidos
            Time *time1 = bdtimes_buscar_por_id(bdt, t1);
            Time *time2 = bdtimes_buscar_por_id(bdt, t2);

            if (time1 && time2) {
                time1->gols_marcados += g1;
                time1->gols_sofridos += g2;
                time2->gols_marcados += g2;
                time2->gols_sofridos += g1;

                if (g1 > g2) {
                    time1->vitorias++;
                    time2->derrotas++;
                } else if (g2 > g1) {
                    time2->vitorias++;
                    time1->derrotas++;
                } else {
                    time1->empates++;
                    time2->empates++;
                }
            }
        }
    }

    fclose(f);
    return 0;
}

/*
 * Verifica se o nome do time (buscado pelo id) começa com o prefixo.
 * Retorna 1 se sim, 0 se não.
 */
static int nome_tem_prefixo(BDTimes *bdt, int id_time, const char *prefixo) {
    Time *t = bdtimes_buscar_por_id(bdt, id_time);
    if (!t) return 0;
    return strncasecmp(t->nome, prefixo, strlen(prefixo)) == 0;
}

/*
 * Consulta e imprime partidas filtradas por prefixo e modo.
 * modo 1: apenas mandante, 2: apenas visitante, 3: qualquer um.
 */
void bdpartidas_consultar(BDPartidas *bd, BDTimes *bdt,
                          const char *prefixo, int modo) {
    int encontrados = 0;

    printf("\n%-4s %-12s %-12s\n", "ID", "Time1", "Time2");
    printf("----------------------------------------\n");

    for (int i = 0; i < bd->quantidade; i++) {
        Partida *p = &bd->partidas[i];
        int bate_t1 = nome_tem_prefixo(bdt, p->id_time1, prefixo);
        int bate_t2 = nome_tem_prefixo(bdt, p->id_time2, prefixo);

        int exibir = 0;
        if (modo == 1 && bate_t1) exibir = 1;
        if (modo == 2 && bate_t2) exibir = 1;
        if (modo == 3 && (bate_t1 || bate_t2)) exibir = 1;

        if (exibir) {
            Time *t1 = bdtimes_buscar_por_id(bdt, p->id_time1);
            Time *t2 = bdtimes_buscar_por_id(bdt, p->id_time2);
            printf("%-4d %-12s %d x %d %-12s\n", p->id, t1 ? t1->nome : "?", p->gols_time1, p->gols_time2, t2 ? t2->nome : "?");
            encontrados++;
        }
    }

    if (encontrados == 0)
        printf("Nenhuma partida encontrada para o prefixo '%s'.\n", prefixo);

    printf("\n");
}
