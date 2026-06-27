#ifndef UI_H
#define UI_H

#include "bdtimes.h"
#include "bdpartidas.h"

/*
 * Modulo ui
 * ----------
 * Concentra todas as funcionalidades acessiveis pelo menu principal do
 * sistema, isolando a logica de interacao com o usuario (leitura de
 * entrada, impressao de telas, confirmacoes) das estruturas de dados
 * propriamente ditas (BDTimes/BDPartidas), que sao apenas consumidas
 * por aqui.
 */

/* Exibe o menu principal e retorna a opcao escolhida pelo usuario (em maiusculo). */
char ui_exibir_menu(void);

/* Funcionalidade 1: consulta de time por nome/prefixo. */
void ui_consultar_time(BDTimes *bdtimes);

/* Funcionalidade 2: consulta de partidas por time mandante/visitante/ambos. */
void ui_consultar_partidas(BDPartidas *bdpartidas, BDTimes *bdtimes);

/* Funcionalidade 3: atualizacao de uma partida existente. */
void ui_atualizar_partida(BDPartidas *bdpartidas, BDTimes *bdtimes);

/* Funcionalidade 4: remocao de uma partida existente. */
void ui_remover_partida(BDPartidas *bdpartidas, BDTimes *bdtimes);

/* Funcionalidade 5: insercao de uma nova partida. */
void ui_inserir_partida(BDPartidas *bdpartidas, BDTimes *bdtimes);

/* Funcionalidade 6: impressao da tabela de classificacao ordenada, com paginacao. */
void ui_imprimir_classificacao(BDTimes *bdtimes);

#endif
