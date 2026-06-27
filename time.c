#include "time.h"
#include <stdio.h>
#include <string.h>

//Cria um time com id e nome fornecidos, com todos resultados zerados
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

//Saldo de gols = Gols Marcados - Gols Sofridos
int time_saldo(const Time *t) {
    return t->gols_marcados - t->gols_sofridos;
}

//Pontos ganhos = 3*vitórias + empates
int time_pontos(const Time *t) {
    return 3 * t->vitorias + t->empates;
}

//Imprime o cabeçalho da tabela de classificação
void time_imprimir_cabecalho(void) {
    printf("%-4s %-12s %4s %4s %4s %4s %4s %4s %4s\n",
           "ID", "Time", "V", "E", "D", "GM", "GS", "S", "PG");
    printf("----------------------------------------------------\n");
}

//Imprime uma linha da tabela com as estatísticas do time
void time_imprimir(const Time *t) {
    printf("%-4d %-12s %4d %4d %4d %4d %4d %4d %4d\n",
           t->id, t->nome,
           t->vitorias, t->empates, t->derrotas,
           t->gols_marcados, t->gols_sofridos,
           time_saldo(t), time_pontos(t));
}
