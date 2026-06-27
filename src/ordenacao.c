#include "ordenacao.h"
#include <stdlib.h>
#include <string.h>

/*
 * Compara dois times segundo o criterio de classificacao do
 * campeonato. Retorna um valor > 0 se `a` deve vir ANTES de `b` na
 * ordenacao decrescente (ou seja, `a` tem melhor desempenho).
 *
 * Criterios, em ordem de prioridade: PG, depois V, depois Saldo de Gols.
 */
static int time_tem_prioridade(const Time *a, const Time *b) {
    int pg_a = time_pontos(a);
    int pg_b = time_pontos(b);
    if (pg_a != pg_b) {
        return pg_a > pg_b;
    }

    if (a->vitorias != b->vitorias) {
        return a->vitorias > b->vitorias;
    }

    int saldo_a = time_saldo(a);
    int saldo_b = time_saldo(b);
    return saldo_a > saldo_b;
}

/* Intercala (merge) dois sub-vetores ja ordenados [inicio..meio] e [meio+1..fim]. */
static void intercalar(Time *vetor[], int inicio, int meio, int fim, Time *temp[]) {
    int i = inicio;
    int j = meio + 1;
    int k = inicio;

    while (i <= meio && j <= fim) {
        if (time_tem_prioridade(vetor[i], vetor[j])) {
            temp[k++] = vetor[i++];
        } else {
            temp[k++] = vetor[j++];
        }
    }
    while (i <= meio) {
        temp[k++] = vetor[i++];
    }
    while (j <= fim) {
        temp[k++] = vetor[j++];
    }

    for (i = inicio; i <= fim; i++) {
        vetor[i] = temp[i];
    }
}

/* Merge Sort recursivo sobre o intervalo [inicio..fim] do vetor de ponteiros. */
static void merge_sort(Time *vetor[], int inicio, int fim, Time *temp[]) {
    if (inicio >= fim) {
        return;
    }

    int meio = inicio + (fim - inicio) / 2;
    merge_sort(vetor, inicio, meio, temp);
    merge_sort(vetor, meio + 1, fim, temp);
    intercalar(vetor, inicio, meio, fim, temp);
}

void ordenar_classificacao(Time *vetor[], int n) {
    if (n <= 1) {
        return;
    }

    Time **temp = (Time **) malloc((size_t) n * sizeof(Time *));
    if (temp == NULL) {
        return; /* Sem memoria para ordenar; mantem a ordem original. */
    }

    merge_sort(vetor, 0, n - 1, temp);
    free(temp);
}
