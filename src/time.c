#include "time.h"
#include <stdio.h>
#include <string.h>

Time time_criar(int id, const char *nome) {
    Time t;
    t.id = id;
    strncpy(t.nome, nome, MAX_NOME - 1);
    t.nome[MAX_NOME - 1] = '\0';
    t.vitorias = 0;
    t.empates = 0;
    t.derrotas = 0;
    t.gols_marcados = 0;
    t.gols_sofridos = 0;
    return t;
}

int time_saldo(const Time *t) {
    return t->gols_marcados - t->gols_sofridos;
}

int time_pontos(const Time *t) {
    return 3 * t->vitorias + t->empates;
}

void time_imprimir_cabecalho(void) {
    printf("%-4s %-12s %4s %4s %4s %4s %4s %4s %4s\n",
           "ID", "Time", "V", "E", "D", "GM", "GS", "S", "PG");
    printf("--------------------------------------------------------\n");
}

void time_imprimir(const Time *t) {
    printf("%-4d %-12s %4d %4d %4d %4d %4d %4d %4d\n",
           t->id, t->nome,
           t->vitorias, t->empates, t->derrotas,
           t->gols_marcados, t->gols_sofridos,
           time_saldo(t), time_pontos(t));
}

void time_aplicar_resultado(Time *t, int gols_marcados, int gols_sofridos, int desfazer) {
    int sinal = desfazer ? -1 : 1;

    t->gols_marcados += sinal * gols_marcados;
    t->gols_sofridos  += sinal * gols_sofridos;

    if (gols_marcados > gols_sofridos) {
        t->vitorias += sinal;
    } else if (gols_marcados < gols_sofridos) {
        t->derrotas += sinal;
    } else {
        t->empates += sinal;
    }
}
