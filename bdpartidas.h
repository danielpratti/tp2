#ifndef BDPARTIDAS_H
#define BDPARTIDAS_H

#include "partida.h"
#include "bdtimes.h"

#define MAX_PARTIDAS 500

//Estrutura que gerencia a coleção de partidas
typedef struct {
    Partida partidas[MAX_PARTIDAS];
    int quantidade;
} BDPartidas;

//Inicializa o banco de dados de partidas (vazio)
void bdpartidas_init(BDPartidas *bd);

/*
 * Carrega partidas do CSV e acumula estatísticas nos times do BDTimes.
 * Retorna 0 em sucesso, -1 em erro.
 */
int bdpartidas_carregar(BDPartidas *bd, BDTimes *bdt, const char *arquivo);

/*
 * Consulta partidas por prefixo de nome.
 * modo: 1 = mandante, 2 = visitante, 3 = qualquer um dos dois.
 * bdt é usado para resolver id -> nome do time.
 */
void bdpartidas_consultar(BDPartidas *bd, BDTimes *bdt,
                          const char *prefixo, int modo);

#endif
