#ifndef PARTIDA_H
#define PARTIDA_H

//Estrutura que representa uma partida entre dois times
typedef struct {
    int id;
    int id_time1;
    int id_time2;
    int gols_time1;
    int gols_time2;
} Partida;

//Cria e retorna uma Partida com os dados fornecidos
Partida partida_criar(int id, int id_time1, int id_time2, int gols_time1, int gols_time2);

#endif
