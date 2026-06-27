#ifndef TIME_H
#define TIME_H

#define MAX_NOME 50

/*
 * TAD Time
 * ---------
 * Representa uma unica equipe de futebol. Encapsula tanto os dados de
 * identificacao (id e nome, lidos do arquivo times.csv) quanto as
 * estatisticas de desempenho acumuladas a partir das partidas disputadas
 * (vitorias, empates, derrotas, gols marcados e gols sofridos).
 *
 * Os dados derivados (saldo de gols e pontos ganhos) NAO sao armazenados
 * diretamente na estrutura: sao calculados sob demanda por funcoes da
 * interface publica, garantindo que nunca fiquem desatualizados.
 */
typedef struct {
    int id;
    char nome[MAX_NOME];
    int vitorias;
    int empates;
    int derrotas;
    int gols_marcados;
    int gols_sofridos;
} Time;

/* Cria e retorna um Time com estatisticas zeradas. */
Time time_criar(int id, const char *nome);

/* Retorna o saldo de gols (gols marcados - gols sofridos). */
int time_saldo(const Time *t);

/* Retorna os pontos ganhos (3 * vitorias + empates). */
int time_pontos(const Time *t);

/* Imprime o cabecalho da tabela de classificacao/consulta. */
void time_imprimir_cabecalho(void);

/* Imprime uma linha da tabela com as estatisticas do time informado. */
void time_imprimir(const Time *t);

/*
 * Atualiza as estatisticas do time considerando o resultado de uma
 * partida em que ele marcou `gols_marcados` e sofreu `gols_sofridos`.
 * Tambem e usada para "desfazer" um resultado quando `desfazer` for 1
 * (necessario para as operacoes de atualizacao/remocao de partidas).
 */
void time_aplicar_resultado(Time *t, int gols_marcados, int gols_sofridos, int desfazer);

#endif
