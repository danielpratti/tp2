#ifndef BDTIMES_H
#define BDTIMES_H

#include "time.h"

/*
 * TAD BDTimes
 * -----------
 * Gerencia a colecao de todos os times do campeonato por meio de uma
 * lista simplesmente encadeada. Cada no da lista guarda uma instancia
 * do TAD Time. A interface publica abstrai os detalhes de implementacao
 * (alocacao dos nos, percurso da lista, etc.), expondo apenas operacoes
 * de alto nivel: carga a partir de CSV, busca, listagem e atualizacao
 * das estatisticas.
 */

typedef struct NoTime {
    Time dado;
    struct NoTime *prox;
} NoTime;

typedef struct {
    NoTime *inicio;
    int total;
} BDTimes;

/* Inicializa um BDTimes vazio (lista com 0 nos). */
void bdtimes_inicializar(BDTimes *bd);

/* Libera toda a memoria alocada pelos nos da lista. */
void bdtimes_destruir(BDTimes *bd);

/*
 * Carrega os times a partir do arquivo CSV indicado (formato ID,Nome).
 * Retorna a quantidade de registros carregados, ou -1 em caso de erro
 * de abertura do arquivo.
 */
int bdtimes_carregar_csv(BDTimes *bd, const char *caminho_arquivo);

/* Salva o estado atual do BDTimes (com as estatisticas) em um CSV. */
int bdtimes_salvar_csv(const BDTimes *bd, const char *caminho_arquivo);

/* Insere um novo time ao final da lista. Retorna 1 em sucesso, 0 em falha de alocacao. */
int bdtimes_inserir(BDTimes *bd, Time t);

/* Busca um time pelo ID. Retorna ponteiro para o Time encontrado, ou NULL. */
Time *bdtimes_buscar_por_id(BDTimes *bd, int id);

/*
 * Busca todos os times cujo nome comeca com o prefixo informado
 * (case-insensitive). Os ponteiros encontrados sao escritos no vetor
 * `resultado` (que deve ter capacidade >= bd->total). Retorna a
 * quantidade de times encontrados.
 */
int bdtimes_buscar_por_prefixo(BDTimes *bd, const char *prefixo, Time *resultado[], int capacidade);

/* Retorna o numero de times cadastrados. */
int bdtimes_total(const BDTimes *bd);

/*
 * Preenche o vetor `vetor` (capacidade >= bdtimes_total(bd)) com
 * ponteiros para todos os times da lista, na ordem em que estao
 * armazenados. Retorna a quantidade de elementos copiados.
 */
int bdtimes_para_vetor(BDTimes *bd, Time *vetor[], int capacidade);

#endif
