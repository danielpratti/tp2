#include "partidas_servico.h"
#include <stdlib.h>

/* Aplica (ou desfaz) o efeito de uma partida nas estatisticas dos dois times envolvidos. */
static void aplicar_efeito_partida(BDTimes *bdtimes, const Partida *p, int desfazer) {
    Time *t1 = bdtimes_buscar_por_id(bdtimes, p->id_time1);
    Time *t2 = bdtimes_buscar_por_id(bdtimes, p->id_time2);

    if (t1 != NULL) {
        time_aplicar_resultado(t1, p->gols_time1, p->gols_time2, desfazer);
    }
    if (t2 != NULL) {
        time_aplicar_resultado(t2, p->gols_time2, p->gols_time1, desfazer);
    }
}

void servico_recalcular_todas_estatisticas(BDPartidas *bdpartidas, BDTimes *bdtimes) {
    int total = bdpartidas_total(bdpartidas);
    Partida **vetor = NULL;

    if (total > 0) {
        vetor = (Partida **) malloc((size_t) total * sizeof(Partida *));
        if (vetor == NULL) {
            return;
        }
        bdpartidas_para_vetor(bdpartidas, vetor, total);

        for (int i = 0; i < total; i++) {
            aplicar_efeito_partida(bdtimes, vetor[i], 0);
        }
        free(vetor);
    }
}

int servico_inserir_partida(BDPartidas *bdpartidas, BDTimes *bdtimes,
                             int id_time1, int id_time2, int gols_time1, int gols_time2) {
    int novo_id = bdpartidas_inserir(bdpartidas, id_time1, id_time2, gols_time1, gols_time2);
    if (novo_id < 0) {
        return -1;
    }

    Partida *p = bdpartidas_buscar_por_id(bdpartidas, novo_id);
    if (p != NULL) {
        aplicar_efeito_partida(bdtimes, p, 0);
    }
    return novo_id;
}

int servico_atualizar_partida(BDPartidas *bdpartidas, BDTimes *bdtimes,
                               int id, int novo_gols_time1, int novo_gols_time2) {
    Partida *p = bdpartidas_buscar_por_id(bdpartidas, id);
    if (p == NULL) {
        return 0;
    }

    /* 1) Desfaz o efeito do placar antigo. */
    aplicar_efeito_partida(bdtimes, p, 1);

    /* 2) Atualiza o registro da partida (mantem campos com sentinela -1). */
    bdpartidas_atualizar(bdpartidas, id, novo_gols_time1, novo_gols_time2);

    /* 3) Aplica o efeito do novo placar. */
    Partida *p_atualizada = bdpartidas_buscar_por_id(bdpartidas, id);
    if (p_atualizada != NULL) {
        aplicar_efeito_partida(bdtimes, p_atualizada, 0);
    }
    return 1;
}

int servico_remover_partida(BDPartidas *bdpartidas, BDTimes *bdtimes, int id) {
    Partida *p = bdpartidas_buscar_por_id(bdpartidas, id);
    if (p == NULL) {
        return 0;
    }

    /* Desfaz o efeito da partida antes de remove-la da lista. */
    aplicar_efeito_partida(bdtimes, p, 1);

    return bdpartidas_remover(bdpartidas, id);
}
