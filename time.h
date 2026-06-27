#ifndef TIME_H
#define TIME_H

#define MAX_NOME 50

//Estrutura que representa um time
typedef struct {
    int id;
    char nome[MAX_NOME];
    int vitorias;
    int empates;
    int derrotas;
    int gols_marcados;
    int gols_sofridos;
} Time;

//Cria e retorna um Time com valores zerados
Time time_criar(int id, const char *nome);

//Retorna o saldo de gols (Gols Marcados - Gols Sofridos)
int time_saldo(const Time *t);

//Retorna os pontos ganhos (3*Vitórias + Empates)
int time_pontos(const Time *t);

//Imprime as estatísticas de um time formatadas
void time_imprimir(const Time *t);

//Imprime o cabeçalho da tabela de classificação
void time_imprimir_cabecalho(void);

#endif
