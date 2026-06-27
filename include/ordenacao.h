#ifndef ORDENACAO_H
#define ORDENACAO_H

#include "time.h"

/*
 * Modulo ordenacao
 * -----------------
 * Implementa a ordenacao da tabela de classificacao. Como o BDTimes e
 * uma lista encadeada, optou-se pelo Merge Sort, um algoritmo estavel
 * e eficiente (O(n log n)) que se adapta naturalmente a listas
 * encadeadas (nao exige acesso indexado/aleatorio aos elementos, ao
 * contrario de algoritmos como QuickSort ou HeapSort).
 *
 * Criterio de ordenacao (decrescente): Pontos Ganhos (PG) > Vitorias
 * (V) > Saldo de Gols (S), conforme especificado no enunciado.
 */

/*
 * Ordena (em ordem decrescente de merito esportivo) um vetor de
 * ponteiros para Time, de tamanho `n`, usando Merge Sort.
 */
void ordenar_classificacao(Time *vetor[], int n);

#endif
