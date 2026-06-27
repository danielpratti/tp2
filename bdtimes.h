#ifndef BDTIMES_H
#define BDTIMES_H

#include "time.h"

#define MAX_TIMES 100

//Estrutura que gerencia a coleção de times
typedef struct {
    Time times[MAX_TIMES];
    int quantidade;
} BDTimes;

//Inicializa o banco de dados de times (vazio)
void bdtimes_init(BDTimes *bd);

//Carrega times do arquivo CSV; retorna 0 em sucesso, -1 em erro
int bdtimes_carregar(BDTimes *bd, const char *arquivo);

//Retorna ponteiro para o time com o id dado, ou NULL se não encontrar
Time *bdtimes_buscar_por_id(BDTimes *bd, int id);

/*
 * Busca times cujo nome começa com o prefixo fornecido.
 * Preenche 'resultados' com ponteiros para os times encontrados.
 * Retorna a quantidade encontrada.
 */
int bdtimes_buscar_por_prefixo(BDTimes *bd, const char *prefixo,
                                Time **resultados, int max_resultados);

//Imprime a tabela de classificação completa (times, ordenados por ID)
void bdtimes_imprimir_tabela(BDTimes *bd);

#endif
