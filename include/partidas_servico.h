#ifndef PARTIDAS_SERVICO_H
#define PARTIDAS_SERVICO_H

#include "bdpartidas.h"
#include "bdtimes.h"

/*
 * Modulo partidas_servico
 * ------------------------
 * Camada de orquestracao entre o TAD BDPartidas e o TAD BDTimes.
 *
 * Os TADs de dados (BDPartidas, BDTimes) sao deliberadamente "ingenuos":
 * cada um conhece apenas sua propria colecao. Este modulo e responsavel
 * por manter a consistencia entre as duas colecoes sempre que uma
 * partida e inserida, atualizada ou removida, recalculando o impacto
 * nas estatisticas (V/E/D/GM/GS) dos times envolvidos.
 */

/* Aplica a todos os times do BDTimes as estatisticas resultantes do BDPartidas completo. */
void servico_recalcular_todas_estatisticas(BDPartidas *bdpartidas, BDTimes *bdtimes);

/* Insere uma nova partida e atualiza as estatisticas dos dois times envolvidos. */
int servico_inserir_partida(BDPartidas *bdpartidas, BDTimes *bdtimes,
                             int id_time1, int id_time2, int gols_time1, int gols_time2);

/*
 * Atualiza o placar de uma partida existente, desfazendo o efeito do
 * placar antigo nas estatisticas e aplicando o efeito do novo placar.
 * Sentinela -1 em novo_gols_time1/novo_gols_time2 mantem o valor atual.
 */
int servico_atualizar_partida(BDPartidas *bdpartidas, BDTimes *bdtimes,
                               int id, int novo_gols_time1, int novo_gols_time2);

/* Remove uma partida, desfazendo seu efeito nas estatisticas dos times envolvidos. */
int servico_remover_partida(BDPartidas *bdpartidas, BDTimes *bdtimes, int id);

#endif
