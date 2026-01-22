#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
#include "../include/equipas.h" 

Equipa tabela[MAX_EQUIPAS];
int totalEquipas = 0;

int procurarID(int id) {
    for (int i = 0; i < totalEquipas; i++) {
        if (tabela[i].id == id) return i; 
    }
    return -1; 
}

int nomesIguais(char *nome1, char *nome2) {
    if (strlen(nome1) != strlen(nome2)) return 0;
    for(int i = 0; nome1[i]; i++){
        if(toupper(nome1[i]) != toupper(nome2[i])) return 0;
    }
    return 1;
}

void ordenarPorPontos() {
    Equipa temp;
    // Algoritmo para ordenar as equipas
    for (int i = 0; i < totalEquipas - 1; i++) {
        for (int j = 0; j < totalEquipas - i - 1; j++) {
            int trocar = 0;
            
            // --- CRITÉRIOS DE DESEMPATE OFICIAIS ---
            // 1.º - Quem tem mais pontos fica à frente
            if (tabela[j+1].pontos > tabela[j].pontos) trocar = 1;
            
            // 2.º - Se os pontos forem iguais, ganha quem tiver melhor Diferença de Golos
            else if (tabela[j+1].pontos == tabela[j].pontos) {
                if (tabela[j+1].dg > tabela[j].dg) trocar = 1;
                
                // 3.º - Se a DG for igual, ganha quem marcou mais Golos (Melhor Ataque)
                else if (tabela[j+1].dg == tabela[j].dg) {
                    if (tabela[j+1].gm > tabela[j].gm) trocar = 1;
                }
            }
            
            if (trocar) {
                temp = tabela[j];
                tabela[j] = tabela[j+1];
                tabela[j+1] = temp;
            }
        }
    }
}

void mostrarClassificacao() {
    ordenarPorPontos(); 
    limparEcra(); 

    if (totalEquipas == 0) {
        printf(CYAN "==============================================================================\n" RESET);
        printf(CYAN "                             CLASSIFICAÇÃO ATUAL                              \n" RESET);
        printf(CYAN "==============================================================================\n\n" RESET);
        printf(RED "Nenhuma equipa registada.\n" RESET);
        return;
    }

    printf(CYAN "==============================================================================\n" RESET);
    printf(CYAN "                             CLASSIFICAÇÃO ATUAL                              \n" RESET);
    printf(CYAN "==============================================================================\n\n" RESET);

    printf(YELLOW " Pos | Equipa              | Pts |  J  |  V  E  D |  GM - GS  |  DG \n" RESET);
    printf("-----+---------------------+-----+-----+----------+-----------+-----\n");
    
    int nChampions = (totalEquipas >= 10) ? 2 : 1;
    int nEuropa    = (totalEquipas >= 10) ? 2 : 1;
    int nDescida   = (totalEquipas >= 10) ? 2 : 1;

    for (int i = 0; i < totalEquipas; i++) {
        char *corLinha = RESET;

        // Aplica a cor consoante a posição atual da equipa
        if (i < nChampions) corLinha = CYAN;
        else if (i < nChampions + nEuropa) corLinha = GREEN;
        else if (i >= totalEquipas - nDescida) corLinha = RED;

        printf("%s  %02d | %-19.19s | %3d | %3d | %2d %2d %2d | %3d - %-3d | %3d " RESET "\n", 
                corLinha, i+1, tabela[i].nome, tabela[i].pontos, tabela[i].jogos,
                tabela[i].vitorias, tabela[i].empates, tabela[i].derrotas,
                tabela[i].gm, tabela[i].gs, tabela[i].dg);
    }
    printf("------------------------------------------------------------------------------\n");
    printf(" LEGENDA: " CYAN "Champions" RESET " | " GREEN "Europa" RESET " | " RED "Descida" RESET "\n");
    printf("------------------------------------------------------------------------------\n");
}

void menuEstatisticas() {
    if (totalEquipas == 0) { printf(RED "Sem dados.\n" RESET); pausar(); return; }
    ordenarPorPontos(); 

    int maxGM = -1, minGS = 1000;
    char atq[MAX_NOME] = "---", def[MAX_NOME] = "---";

    for(int i=0; i<totalEquipas; i++) {
        if(tabela[i].gm > maxGM) { maxGM = tabela[i].gm; strcpy(atq, tabela[i].nome); }
        if(tabela[i].gs < minGS) { minGS = tabela[i].gs; strcpy(def, tabela[i].nome); }
    }

    limparEcra();
    printf(CYAN "=====================================================\n" RESET);
    printf(CYAN "                    ESTATÍSTICAS                     \n" RESET);
    printf(CYAN "=====================================================\n\n" RESET);

    printf(GREEN "TOP 3:\n" RESET);
    for(int i=0; i<3 && i<totalEquipas; i++) {
        printf(" %d. %s (%d pts)\n", i+1, tabela[i].nome, tabela[i].pontos);
    }
    printf(YELLOW "\nDESTAQUES:\n" RESET);
    printf(" Melhor Ataque: " CYAN "%s" RESET " (%d Golos)\n", atq, maxGM);
    printf(" Melhor Defesa: " CYAN "%s" RESET " (%d Golos)\n", def, (minGS==1000)?0:minGS);
    
    printf("-----------------------------------------------------\n");
    pausar();
}

void reiniciarEpoca() {
    for(int i=0; i<totalEquipas; i++) {
        tabela[i].pontos = 0; tabela[i].jogos = 0;
        tabela[i].vitorias = 0; tabela[i].empates = 0; tabela[i].derrotas = 0;
        tabela[i].gm = 0; tabela[i].gs = 0; tabela[i].dg = 0;
    }
    guardarEquipas();
}

void criarEquipa() {
    limparEcra(); 
    printf(CYAN "=====================================================\n" RESET);
    printf(CYAN "                  CRIAR NOVA EQUIPA                  \n" RESET);
    printf(CYAN "=====================================================\n\n" RESET);

    if (totalEquipas >= MAX_EQUIPAS) { printf(RED "Limite atingido!\n" RESET); return; }
    
    mostrarListaIDs();
    printf("\nNome da nova equipa (0 para sair): "); 
    
    char nomeTemp[MAX_NOME];
    fgets(nomeTemp, MAX_NOME, stdin);
    nomeTemp[strcspn(nomeTemp, "\n")] = 0; 

    if (strcmp(nomeTemp, "0") == 0 || strlen(nomeTemp) == 0) return;

    for(int i=0; i<totalEquipas; i++) {
        if (nomesIguais(tabela[i].nome, nomeTemp)) {
            printf(RED "Erro: Nome duplicado!\n" RESET); return;
        }
    }

    tabela[totalEquipas].id = totalEquipas + 1;
    strcpy(tabela[totalEquipas].nome, nomeTemp);
    tabela[totalEquipas].pontos = 0; tabela[totalEquipas].jogos = 0;
    tabela[totalEquipas].vitorias = 0; tabela[totalEquipas].empates = 0; tabela[totalEquipas].derrotas = 0;
    tabela[totalEquipas].gm = 0; tabela[totalEquipas].gs = 0; tabela[totalEquipas].dg = 0;
    
    totalEquipas++;
    guardarEquipas();
    printf(GREEN "\nSucesso! Equipa registada com ID %02d\n" RESET, totalEquipas);
}

void editarEquipa() {
    limparEcra();
    printf(CYAN "=====================================================\n" RESET);
    printf(CYAN "                    EDITAR EQUIPA                    \n" RESET);
    printf(CYAN "=====================================================\n\n" RESET);

    mostrarListaIDs();
    printf("\nID para editar: "); 
    int id = lerNumero();
    int pos = procurarID(id);
    if (pos == -1) { printf(RED "Erro: ID não existe.\n" RESET); return; }

    printf("Nome atual: %s\n", tabela[pos].nome);
    printf("Novo nome: ");
    char novo[MAX_NOME];
    fgets(novo, MAX_NOME, stdin);
    novo[strcspn(novo, "\n")] = 0; 

    if (strlen(novo) > 0) {
        strcpy(tabela[pos].nome, novo);
        guardarEquipas();
        printf(GREEN "\n>> Atualizado com sucesso.\n" RESET);
    }
}

void apagarEquipa() {
    limparEcra();
    printf(CYAN "=====================================================\n" RESET);
    printf(CYAN "                    APAGAR EQUIPA                    \n" RESET);
    printf(CYAN "=====================================================\n\n" RESET);

    int campeonatoComecou = 0;
    for(int i=0; i<totalEquipas; i++) if(tabela[i].jogos > 0) campeonatoComecou = 1;
    
    if (campeonatoComecou) {
        printf(RED "Erro: O campeonato já tem jogos registados.\n" RESET);
        printf(RED "Não é possível apagar equipas para não corromper o histórico.\n" RESET);
        return; 
    }

    mostrarListaIDs();
    printf("\nID a apagar: "); 
    int id = lerNumero();
    int pos = procurarID(id);
    if (pos == -1) return;
    
    for (int i = pos; i < totalEquipas - 1; i++) tabela[i] = tabela[i+1];
    totalEquipas--;
    
    for (int i = 0; i < totalEquipas; i++) tabela[i].id = i + 1;
    
    guardarEquipas();
    printf(GREEN "\n>> Equipa apagada com sucesso.\n" RESET);
}

void mostrarListaIDs() {
    if (totalEquipas == 0) return;
    Equipa temp[MAX_EQUIPAS];
    for(int k=0; k<totalEquipas; k++) temp[k] = tabela[k];
    
    for (int i = 0; i < totalEquipas - 1; i++) {
        for (int j = 0; j < totalEquipas - i - 1; j++) {
            if (temp[j].id > temp[j+1].id) {
                Equipa t = temp[j]; temp[j] = temp[j+1]; temp[j+1] = t;
            }
        }
    }

    printf(CYAN "-----------------------------------------------------\n" RESET);
    printf(CYAN "                  LISTA DE EQUIPAS                   \n" RESET);
    printf(CYAN "-----------------------------------------------------\n" RESET);
    
    for (int i = 0; i < totalEquipas; i++) {
        printf(CYAN "[%02d]" RESET " %-20.20s ", temp[i].id, temp[i].nome);
        if ((i + 1) % 2 == 0) printf("\n");
    }
    if (totalEquipas % 2 != 0) printf("\n");
    printf("-----------------------------------------------------\n");
}

void guardarEquipas() {
    FILE *f = fopen(EQUIPAS, "w");
    if (f == NULL) return;
    fprintf(f, "TOTAL DE EQUIPAS: %d\n", totalEquipas);
    fprintf(f, "ID;Nome;Pontos;Jogos;V;E;D;GM;GS;DG\n");
    for (int i = 0; i < totalEquipas; i++) {
        fprintf(f, "%d;%s;%d;%d;%d;%d;%d;%d;%d;%d\n",
                tabela[i].id, tabela[i].nome, tabela[i].pontos, tabela[i].jogos,
                tabela[i].vitorias, tabela[i].empates, tabela[i].derrotas,
                tabela[i].gm, tabela[i].gs, tabela[i].dg);
    }
    fclose(f);
}

void carregarEquipas() {
    FILE *f = fopen(EQUIPAS, "r");
    if (f == NULL) return; 
    if (fscanf(f, "TOTAL DE EQUIPAS: %d", &totalEquipas) != 1) { totalEquipas = 0; fclose(f); return; }
    
    if (totalEquipas > MAX_EQUIPAS) totalEquipas = MAX_EQUIPAS;

    char lixo[200]; fgets(lixo, 200, f); fgets(lixo, 200, f); 
    for (int i = 0; i < totalEquipas; i++) {
        fscanf(f, " %d;%49[^;];%d;%d;%d;%d;%d;%d;%d;%d",
               &tabela[i].id, tabela[i].nome, &tabela[i].pontos, &tabela[i].jogos,
               &tabela[i].vitorias, &tabela[i].empates, &tabela[i].derrotas,
               &tabela[i].gm, &tabela[i].gs, &tabela[i].dg);
        tabela[i].nome[strcspn(tabela[i].nome, "\n")] = 0; 
    }
    fclose(f);
}