#ifndef EQUIPAS_H
#define EQUIPAS_H

#include "dados.h"

extern Equipa tabela[MAX_EQUIPAS];
extern int totalEquipas;

// Gestão das Equipas
void criarEquipa();
void editarEquipa();
void apagarEquipa();

// Visualização
void mostrarClassificacao();
void mostrarListaIDs();
void menuEstatisticas();

// Ficheiros
void guardarEquipas();
void carregarEquipas();

// Auxiliares
void reiniciarEpoca();
int procurarID(int id);
void ordenarPorPontos();

#endif