#ifndef JOGOS_H
#define JOGOS_H

#include "dados.h"

void menuJogos();

void carregarJogos();
void guardarJogos(int idCasa, int idFora, int gC, int gF);
void listarJogosRealizados();

void limparHistoricoJogos();

#endif