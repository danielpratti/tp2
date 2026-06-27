#include "partida.h"

//Cria uma partida preenchendo todos os campos
Partida partida_criar(int id, int id_time1, int id_time2, int gols_time1, int gols_time2) {
    Partida p;
    p.id = id;
    p.id_time1 = id_time1;
    p.id_time2 = id_time2;
    p.gols_time1 = gols_time1;
    p.gols_time2 = gols_time2;
    return p;
}
