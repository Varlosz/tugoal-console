#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../include/jogos.h"
#include "../include/equipas.h" 

int jogosEntre[MAX_EQUIPAS][MAX_EQUIPAS] = {0};

void guardarJogos(int idCasa, int idFora, int gC, int gF) {
    FILE *f = fopen(JOGOS, "a");
    if (f == NULL) return;
    char nomeCasa[MAX_NOME] = "?", nomeFora[MAX_NOME] = "?";
    int posC = procurarID(idCasa), posF = procurarID(idFora);
    if (posC != -1) strcpy(nomeCasa, tabela[posC].nome);
    if (posF != -1) strcpy(nomeFora, tabela[posF].nome);
    fprintf(f, "%d;%s;%d;%d;%s;%d\n", idCasa, nomeCasa, gC, gF, nomeFora, idFora);
    fclose(f);
}

void carregarJogos() {
    for(int i=0; i<MAX_EQUIPAS; i++) for(int j=0; j<MAX_EQUIPAS; j++) jogosEntre[i][j] = 0;
    FILE *f = fopen(JOGOS, "r");
    
    int ficheiroExiste = (f != NULL);
    int haEquipasComJogos = 0;
    for(int i=0; i<totalEquipas; i++) if(tabela[i].jogos > 0) haEquipasComJogos = 1;

    if (!ficheiroExiste && haEquipasComJogos) {
        limparEcra();
        printf(CYAN "=====================================================\n" RESET);
        printf(RED  "             ERRO: FICHEIROS INCONSISTENTES          \n" RESET);
        printf(CYAN "=====================================================\n\n" RESET);
        printf("  Inconsistência: Ficheiro de jogos em falta.\n");
        printf("  A reiniciar época...\n");
        printf("-----------------------------------------------------\n");
        pausar(); reiniciarEpoca(); limparHistoricoJogos(); return; 
    }
    if (!ficheiroExiste) return;

    int idC, idF, gC, gF;
    char lixo[MAX_NOME]; 
    while (fscanf(f, "%d;%49[^;];%d;%d;%49[^;];%d", &idC, lixo, &gC, &gF, lixo, &idF) == 6) {
        int posC = procurarID(idC), posF = procurarID(idF);
        if (posC != -1 && posF != -1) {
            jogosEntre[posC][posF]++; jogosEntre[posF][posC]++;
        }
    }
    fclose(f);
}

void limparHistoricoJogos() {
    for(int i=0; i<MAX_EQUIPAS; i++) for(int j=0; j<MAX_EQUIPAS; j++) jogosEntre[i][j] = 0;
    FILE *f = fopen(JOGOS, "w"); if (f) fclose(f);
}

// CORREÇÃO: Alinhamento ajustado para 53 caracteres (20 nome + 3 sep + 7 score + 3 sep + 20 nome)
void imprimirJogoColorido(char* nCasa, int gC, int gF, char* nFora) {
    char *cor = (gC > gF) ? GREEN : (gC == gF) ? YELLOW : RED;
    printf("%20.20s | %s%2d - %-2d%s | %-20.20s\n", 
            nCasa, cor, gC, gF, RESET, nFora);
}

void listarJogosRealizados() {
    FILE *f = fopen(JOGOS, "r");
    if (f == NULL) { printf(YELLOW "Sem jogos.\n" RESET); pausar(); return; }

    limparEcra();
    printf(CYAN "=====================================================\n" RESET);
    printf(CYAN "            HISTÓRICO DE JOGOS POR JORNADA           \n" RESET);
    printf(CYAN "=====================================================\n\n" RESET);

    int idC, idF, gC, gF; char nC[MAX_NOME], nF[MAX_NOME];
    int contador = 0, jpJ = (totalEquipas > 0) ? (totalEquipas / 2) : 1;

    while (fscanf(f, "%d;%49[^;];%d;%d;%49[^;];%d\n", &idC, nC, &gC, &gF, nF, &idF) == 6) {
        if (contador % jpJ == 0) {
            printf(YELLOW "\n=====================================================\n" RESET);
            printf(CYAN "                    JORNADA %d\n" RESET, (contador / jpJ) + 1);
            printf(YELLOW "=====================================================\n\n" RESET);
            
            // CORREÇÃO: Título alinhado com 53 caracteres
            printf("        Casa         |   RES   |         Fora        \n");
            // CORREÇÃO: Traços alinhados (21 + 1 + 9 + 1 + 21 = 53)
            printf("---------------------+---------+---------------------\n");
        }
        nC[strcspn(nC, "\n")] = 0; nF[strcspn(nF, "\n")] = 0;
        imprimirJogoColorido(nC, gC, gF, nF);
        contador++;
    }
    printf("-----------------------------------------------------\n");
    printf("Total: %d jogos.\n", contador);
    fclose(f);
    pausar();
}

void processarResultado(int posC, int posF, int gC, int gF) {
    Equipa *eC = &tabela[posC], *eF = &tabela[posF];
    eC->jogos++; eF->jogos++;
    eC->gm += gC; eC->gs += gF; eF->gm += gF; eF->gs += gC;
    eC->dg = eC->gm - eC->gs; eF->dg = eF->gm - eF->gs;

    if (gC > gF) { eC->vitorias++; eC->pontos += 3; eF->derrotas++; }
    else if (gF > gC) { eF->vitorias++; eF->pontos += 3; eC->derrotas++; }
    else { eC->empates++; eF->empates++; eC->pontos++; eF->pontos++; }

    jogosEntre[posC][posF]++; jogosEntre[posF][posC]++;
    guardarEquipas(); guardarJogos(eC->id, eF->id, gC, gF);
}

int campeonatoTerminado() {
    if (totalEquipas < 2) return 0;
    for (int i = 0; i < totalEquipas; i++) {
        for (int j = 0; j < totalEquipas; j++) {
            if (i != j && jogosEntre[i][j] < 2) return 0;
        }
    }
    return 1;
}

void registarManual() {
    limparEcra(); 
    printf(CYAN "=====================================================\n" RESET);
    printf(CYAN "              REGISTAR RESULTADO MANUAL              \n" RESET);
    printf(CYAN "=====================================================\n\n" RESET);

    if (campeonatoTerminado()) {
        printf(YELLOW "O campeonato terminou!\n" RESET);
        pausar();               
        mostrarClassificacao(); 
        pausar();               
        return;
    }

    if (totalEquipas < 2) { 
        printf(RED "Mínimo 2 equipas.\n" RESET); 
        pausar();
        return; 
    }
    
    // Passo 1: Escolher as equipas
    mostrarListaIDs();
    printf("\nID Casa: "); int idC = lerNumero(); int pC = procurarID(idC);
    if(pC == -1) return;
    
    printf("ID Fora: "); int idF = lerNumero(); int pF = procurarID(idF);
    if(pF == -1 || idF == idC) { printf(RED "Inválido.\n" RESET); return; }

    if(jogosEntre[pC][pF] >= 2) { printf(RED "Já jogaram 2x.\n" RESET); pausar(); return; }

    // Passo 2: Ecrã de "VS" para dar mais emoção e clareza
    limparEcra();
    printf(CYAN "=====================================================\n" RESET);
    printf(CYAN "                  DEFINIR RESULTADO                  \n" RESET);
    printf(CYAN "=====================================================\n\n" RESET);
    
    printf("        %s   vs   %s        \n", tabela[pC].nome, tabela[pF].nome);
    printf("-----------------------------------------------------\n\n");

    printf("Golos de " CYAN "%s" RESET ": ", tabela[pC].nome); 
    int gC = lerNumero();
    
    printf("Golos de " CYAN "%s" RESET ": ", tabela[pF].nome); 
    int gF = lerNumero();
    
    if (gC < 0 || gF < 0) return;

    processarResultado(pC, pF, gC, gF);

    // Passo 3: Mostrar o resultado final formatado e limpo
    limparEcra();
    printf(CYAN "=====================================================\n" RESET);
    printf(CYAN "                RESULTADO REGISTADO                  \n" RESET);
    printf(CYAN "=====================================================\n\n" RESET);

    printf("        Casa         |   RES   |         Fora        \n");
    printf("---------------------+---------+---------------------\n");
    imprimirJogoColorido(tabela[pC].nome, gC, gF, tabela[pF].nome);
    printf("-----------------------------------------------------\n");
    
    pausar();
}

typedef struct { int c; int f; } ParJogo;

void baralharJogos(ParJogo *array, int n) {
    for (int i = 0; i < n; i++) {
        int r = rand() % n; ParJogo temp = array[i]; array[i] = array[r]; array[r] = temp;
    }
}

void simularProximos() {
    limparEcra();
    printf(CYAN "=====================================================\n" RESET);
    printf(CYAN "                    SIMULAR JORNADA                  \n" RESET);
    printf(CYAN "=====================================================\n\n" RESET);

    if (campeonatoTerminado()) {
        printf(YELLOW "O campeonato terminou!\n" RESET);
        pausar();               
        mostrarClassificacao(); 
        pausar();               
        return;
    }

    if (totalEquipas < 2) { 
        printf(RED "Mínimo 2 equipas.\n" RESET); 
        pausar(); 
        return; 
    }

    // CORREÇÃO: Cabeçalhos alinhados com 53 caracteres
    printf("        Casa         |   RES   |         Fora        \n");
    printf("---------------------+---------+---------------------\n");

    int minJ=1000, maxJ=0;
    for(int i=0; i<totalEquipas; i++) {
        if(tabela[i].jogos > maxJ) maxJ=tabela[i].jogos;
        if(tabela[i].jogos < minJ) minJ=tabela[i].jogos;
    }

    int bloqueado[MAX_EQUIPAS] = {0};
    for(int i=0; i<totalEquipas; i++) if(minJ != maxJ && tabela[i].jogos == maxJ) bloqueado[i]=1;

    ParJogo possiveis[MAX_JOGOS]; int countP = 0;
    for (int i = 0; i < totalEquipas; i++) 
        for (int j = 0; j < totalEquipas; j++) 
            if (i != j && jogosEntre[i][j] < 2) { possiveis[countP].c = i; possiveis[countP].f = j; countP++; }

    if (countP == 0) { 
        printf(YELLOW "O campeonato terminou!\n" RESET); 
        pausar();               
        mostrarClassificacao();
        pausar(); 
        return; 
    }

    ParJogo melhorSelecao[MAX_EQUIPAS]; int melhorQtd = 0;

    for(int k=0; k<1000; k++) {
        baralharJogos(possiveis, countP);
        ParJogo atual[MAX_EQUIPAS]; int qtd=0, used[MAX_EQUIPAS]={0};
        for (int m=0; m<countP; m++) {
            int i=possiveis[m].c, j=possiveis[m].f;
            if(!used[i] && !used[j] && !bloqueado[i] && !bloqueado[j]) {
                atual[qtd++] = possiveis[m]; used[i]=1; used[j]=1;
            }
        }
        if(qtd > melhorQtd) { melhorQtd=qtd; for(int m=0; m<qtd; m++) melhorSelecao[m]=atual[m]; }
        if(melhorQtd == totalEquipas/2) break;
    }

    if(melhorQtd == 0) {
        for(int k=0; k<1000; k++) {
            baralharJogos(possiveis, countP);
            ParJogo atual[MAX_EQUIPAS]; int qtd=0, used[MAX_EQUIPAS]={0};
            for (int m=0; m<countP; m++) {
                int i=possiveis[m].c, j=possiveis[m].f;
                if(!used[i] && !used[j]) {
                    atual[qtd++] = possiveis[m]; used[i]=1; used[j]=1;
                }
            }
            if(qtd > melhorQtd) { melhorQtd=qtd; for(int m=0; m<qtd; m++) melhorSelecao[m]=atual[m]; }
        }
    }

    if (melhorQtd > 0) {
        for(int k=0; k < melhorQtd; k++) {
            int i = melhorSelecao[k].c, j = melhorSelecao[k].f;
            int gC = rand() % 5, gF = rand() % 5; if (rand()%10 < 3) gC++; 
            processarResultado(i, j, gC, gF);
            imprimirJogoColorido(tabela[i].nome, gC, gF, tabela[j].nome);
        }
        printf(GREEN "\n>> Jornada completada! %d novos jogos realizados.\n" RESET, melhorQtd);
    } else {
        printf(YELLOW "Nenhum jogo possível nesta ronda.\n" RESET);
    }
    pausar();
}

void menuJogos() {
    int opcao;
    do {
        limparEcra();
        printf(CYAN "=====================================================\n" RESET);
        printf(CYAN "                    MENU DE JOGOS                    \n" RESET);
        printf(CYAN "=====================================================\n\n" RESET);
        printf("-----------------------------------------------------\n");
        printf("  " YELLOW "1" RESET " - Registar Resultado Manual\n");
        printf("  " YELLOW "2" RESET " - Simular Jornada\n"); 
        printf("  " YELLOW "3" RESET " - Ver Histórico de Jogos\n");
        printf("  " YELLOW "0" RESET " - Voltar\n");
        printf("-----------------------------------------------------\n");
        printf(YELLOW "Opção: " RESET);
        opcao = lerNumero();
        switch (opcao) {
            case 1: registarManual(); break;
            case 2: simularProximos(); break; 
            case 3: listarJogosRealizados(); break;
            case 0: break;
            default: printf(RED "Inválido.\n" RESET); Sleep(500);
        }
    } while (opcao != 0);
}