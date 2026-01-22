#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include "../include/dados.h"
#include "../include/equipas.h"
#include "../include/jogos.h" 

void limparEcra() { 
    system("cls"); 
}

void pausar() {
    printf(CYAN "\n[ENTER] para continuar..." RESET);
    char c;
    // while para limpar o teclado para evitar saltos automáticos
    while ((c = getchar()) != '\n' && c != EOF); 
}

int lerNumero() {
    char buffer[100];
    int numero;
    // fgets é usado para evitar loops infinitos quando o ut digita com letras
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (sscanf(buffer, "%d", &numero) == 1) return numero;
    }
    return -1; 
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    CreateDirectory("data", NULL);
    srand(time(NULL)); 
    
    carregarEquipas(); 
    carregarJogos(); 

    int opcao;

    do {
        limparEcra();
        printf(CYAN "=====================================================\n" RESET);
        printf(CYAN "                    T U G O A L                      \n" RESET);
        printf(CYAN "            Sistema de Gestão de Campeonato          \n" RESET);
        printf(CYAN "=====================================================\n\n" RESET);

        printf(GREEN "                   MENU PRINCIPAL                    \n" RESET);
        printf("-----------------------------------------------------\n");
        printf("  " YELLOW "1" RESET " - Gestão de Equipas\n");
        printf("  " YELLOW "2" RESET " - Jogos (Registar / Simular / Histórico)\n");
        printf("  " YELLOW "3" RESET " - Classificação\n");
        printf("  " YELLOW "4" RESET " - Estatísticas\n");
        printf("  " RED    "5" RESET " - Reiniciar Época\n"); 
        printf("  " YELLOW "0" RESET " - Sair\n");
        printf("-----------------------------------------------------\n");
        printf(YELLOW "Opção: " RESET);
        
        opcao = lerNumero();

        switch (opcao) {
            case 1: 
                {
                    int sub;
                    do {
                        limparEcra();
                        printf(CYAN "=====================================================\n" RESET);
                        printf(CYAN "                  GESTÃO DE EQUIPAS                  \n" RESET);
                        printf(CYAN "=====================================================\n" RESET);
                        mostrarListaIDs(); 
                        printf("\n");
                        printf("-----------------------------------------------------\n");
                        printf("  " YELLOW "1" RESET " - Criar Nova Equipa\n");
                        printf("  " YELLOW "2" RESET " - Editar Nome da Equipa\n");
                        printf("  " YELLOW "3" RESET " - Apagar Equipa\n");
                        printf("  " YELLOW "0" RESET " - Voltar\n");
                        printf("-----------------------------------------------------\n");
                        printf(YELLOW "Opção: " RESET);
                        
                        sub = lerNumero();
                        switch (sub) {
                            case 1: criarEquipa(); pausar(); break;
                            case 2: editarEquipa(); pausar(); break;
                            case 3: apagarEquipa(); pausar(); break;
                            case 0: break;
                            default: printf(RED "Opção inválida!\n" RESET); Sleep(800);
                        }
                    } while (sub != 0);
                }
                break;
            case 2: menuJogos(); break;
            case 3: limparEcra(); mostrarClassificacao(); pausar(); break;
            case 4: menuEstatisticas(); break;
            case 5: 
                limparEcra();
                printf(CYAN "=====================================================\n" RESET);
                printf(RED  "               ATENÇÃO: NOVA ÉPOCA                   \n" RESET);
                printf(CYAN "=====================================================\n\n" RESET);
                printf("  Isto vai colocar todos os pontos a zero e apagar   \n");
                printf("  o histórico de jogos.                              \n\n");
                printf("  As equipas mantêm-se, mas o campeonato recomeça.   \n\n");
                printf("-----------------------------------------------------\n");
                printf(RED "  Tem a certeza absoluta? (1-Sim / 0-Não): " RESET);
                if (lerNumero() == 1) {
                    reiniciarEpoca();       
                    limparHistoricoJogos(); 
                    printf(GREEN "\n>> Época reiniciada com sucesso! Boa sorte.\n" RESET);
                } else {
                    printf(YELLOW "\nOperação cancelada.\n" RESET);
                }
                pausar();
                break;
            case 0:
                limparEcra();
                printf(CYAN "=====================================================\n" RESET);
                printf(CYAN "                     A SAIR...                       \n" RESET);
                printf(CYAN "=====================================================\n\n" RESET);
                printf(GREEN "         Obrigado por usar o sistema TUGOAL!\n" RESET);
                printf(GREEN "                 Até à próxima!                     \n" RESET);
                printf(CYAN "\n-----------------------------------------------------\n" RESET);
                Sleep(1500);
                break;
            default: printf(RED "Opção inválida! Tente novamente.\n" RESET); Sleep(1000);
        }
    } while (opcao != 0);
    return 0;
}