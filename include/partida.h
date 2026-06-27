#ifndef PARTIDA_H
#define PARTIDA_H

/*
 * TAD Partida
 * -----------
 * Representa as informacoes de um unico jogo de futebol: o identificador
 * da partida, os IDs dos dois times envolvidos e a quantidade de gols
 * marcada por cada um. Os nomes dos times nao sao armazenados aqui -
 * apenas seus IDs - para evitar duplicidade de informacao; a obtencao
 * do nome e feita consultando o BDTimes quando necessario (ex.: para
 * impressao na tela).
 */
typedef struct {
    int id;
    int id_time1;
    int id_time2;
    int gols_time1;
    int gols_time2;
} Partida;

/* Cria e retorna uma Partida com os dados informados. */
Partida partida_criar(int id, int id_time1, int id_time2, int gols_time1, int gols_time2);

#endif
