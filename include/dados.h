#ifndef DADOS_H
#define DADOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h> 

// Definições de Cores
#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define RED     "\033[31m"
#define CYAN    "\033[36m"

// Configurações do Programa
#define MAX_NOME 50
#define MAX_EQUIPAS 18
#define MAX_JOGOS 500
#define EQUIPAS "data/equipas.txt"
#define JOGOS   "data/jogos.txt"

typedef struct {
    int id;
    char nome[MAX_NOME];
    int pontos;
    int jogos;
    int vitorias;
    int empates;
    int derrotas;
    int gm;  // Golos Marcados
    int gs;  // Golos Sofridos
    int dg;  // Diferença de Golos
} Equipa;

typedef struct {
    int idCasa;
    int idFora;
    int golosCasa;
    int golosFora;
} Jogo;

// Funções utilitárias partilhadas
void limparEcra();
void pausar();
int lerNumero();

#endif