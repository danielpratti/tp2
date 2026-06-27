#ifndef BDPARTIDAS_H
#define BDPARTIDAS_H

#include "partida.h"
#include "bdtimes.h"

/*
 * TAD BDPartidas
 * --------------
 * Gerencia a lista simplesmente encadeada de todas as partidas
 * cadastradas no sistema. Alem das operacoes de consulta e listagem,
 * oferece as funcionalidades de manutencao exigidas na Parte II:
 * inserir, atualizar e remover partidas.
 *
 * Importante: este TAD NAO recalcula as estatisticas dos times por
 * conta propria. Quem orquestra a sincronizacao entre uma operacao de
 * insercao/atualizacao/remocao de partida e o impacto nas estatisticas
 * do BDTimes e a camada de aplicacao (ver modulo `partidas_servico`),
 * mantendo este TAD focado exclusivamente na gestao da colecao de
 * partidas (responsabilidade unica).
 */

typedef struct NoPartida {
    Partida dado;
    struct NoPartida *prox;
} NoPartida;

typedef struct {
    NoPartida *inicio;
    int total;
    int proximo_id; /* contador para geracao automatica de IDs (autoincremento) */
} BDPartidas;

/* Modos de consulta de partidas por time, usados em bdpartidas_buscar_por_time. */
typedef enum {
    MODO_MANDANTE = 1,
    MODO_VISITANTE = 2,
    MODO_AMBOS = 3
} ModoConsultaPartida;

/* Inicializa um BDPartidas vazio. */
void bdpartidas_inicializar(BDPartidas *bd);

/* Libera toda a memoria alocada pelos nos da lista. */
void bdpartidas_destruir(BDPartidas *bd);

/*
 * Carrega as partidas a partir do CSV indicado (formato
 * ID,Time1,Time2,GolsTime1,GolsTime2, onde Time1/Time2 sao IDs).
 * Retorna a quantidade de registros carregados, ou -1 em erro de
 * abertura do arquivo.
 */
int bdpartidas_carregar_csv(BDPartidas *bd, const char *caminho_arquivo);

/* Salva o estado atual do BDPartidas em um arquivo CSV. */
int bdpartidas_salvar_csv(const BDPartidas *bd, const char *caminho_arquivo);

/*
 * Insere uma nova partida na lista. O campo `id` da partida informada
 * e ignorado: um novo ID e gerado automaticamente (autoincremento) e
 * atribuido ao no inserido. Retorna o ID gerado em sucesso, ou -1 em
 * falha de alocacao.
 */
int bdpartidas_inserir(BDPartidas *bd, int id_time1, int id_time2, int gols_time1, int gols_time2);

/* Busca uma partida pelo ID. Retorna ponteiro para a Partida, ou NULL se nao encontrada. */
Partida *bdpartidas_buscar_por_id(BDPartidas *bd, int id);

/*
 * Busca partidas cujo time mandante e/ou visitante tenha nome iniciado
 * pelo prefixo informado (case-insensitive), de acordo com o modo
 * escolhido (mandante, visitante ou ambos). Os ponteiros encontrados
 * sao escritos em `resultado` (capacidade >= bd->total). Retorna a
 * quantidade de partidas encontradas. Necessita do BDTimes para
 * resolver os nomes dos times a partir dos IDs armazenados na partida.
 */
int bdpartidas_buscar_por_time(BDPartidas *bd, BDTimes *bdtimes, const char *prefixo,
                                ModoConsultaPartida modo, Partida *resultado[], int capacidade);

/*
 * Atualiza o placar (gols) de uma partida existente, identificada por
 * id. Os parametros novo_gols_time1/novo_gols_time2 aceitam valores
 * negativos para indicar "manter o valor atual" (sentinela -1, ja que
 * gols nao podem ser negativos). Retorna 1 em sucesso, 0 se o ID nao
 * foi encontrado.
 */
int bdpartidas_atualizar(BDPartidas *bd, int id, int novo_gols_time1, int novo_gols_time2);

/* Remove a partida com o ID informado. Retorna 1 em sucesso, 0 se nao encontrada. */
int bdpartidas_remover(BDPartidas *bd, int id);

/* Retorna o numero de partidas cadastradas. */
int bdpartidas_total(const BDPartidas *bd);

/*
 * Preenche o vetor `vetor` (capacidade >= bdpartidas_total(bd)) com
 * ponteiros para todas as partidas da lista, na ordem em que estao
 * armazenadas. Retorna a quantidade de elementos copiados.
 */
int bdpartidas_para_vetor(BDPartidas *bd, Partida *vetor[], int capacidade);

#endif
