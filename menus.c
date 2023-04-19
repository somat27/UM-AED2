#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <conio.h>

#include "estruturas.h"
#include "menus.h"
#include "medicos.h"
#include "utentes.h"
#include "base_dados.h"
#include "fila.h"

void Menu_Principal(){
	int opcao = 1;
    int tecla;

    while (1) {
        system("cls");
        ASCII_Print(1); 
        printf("\n\n");
        printf("\t%s  Atendimento ao Utente\n", opcao == 1 ? ">": " ");
        printf("\t%s  Menu Utentes\n", opcao == 2 ? ">": " ");
        printf("\t%s  Menu Medicos\n", opcao == 3 ? ">": " ");
        printf("\n\t%s  Fechar Programa\n", opcao == 4 ? ">": " ");

        tecla = getch();

        if (tecla == 224) {
            tecla = getch();

            switch (tecla) {
                case 72: 
                    opcao = opcao == 1 ? 4 : opcao - 1; 
                    break;
                case 80: 
                    opcao = opcao == 4 ? 1 : opcao + 1;
                    break;
            }
        } else if (tecla == 13) {
        	switch (opcao) {
                case 1:
                    Menu_Atendimento(); 
                    break;
                case 2: 
                    Menu_Utentes(); 
                    break;
                case 3: 
                    Menu_Medicos(); 
                    break;
                case 4: 
                    Sair_Programa(); 
                    break;
            }
            break;
        }
    }
}

void Sair_Programa(){   
	exit(0);
}

void ASCII_Print(int a){
	fflush(stdin);
	printf("\n");
	if(a==1){
		printf("\n\t\t\t ¦¦¦¦¦¦¦  ¦¦¦¦¦¦   ¦¦¦¦¦  ¦¦    ¦¦");
		printf("\n\t\t\t ¦¦      ¦¦       ¦¦   ¦¦ ¦¦    ¦¦");
		printf("\n\t\t\t ¦¦¦¦¦¦¦ ¦¦   ¦¦¦ ¦¦¦¦¦¦¦ ¦¦    ¦¦");
		printf("\n\t\t\t      ¦¦ ¦¦    ¦¦ ¦¦   ¦¦ ¦¦    ¦¦");
		printf("\n\t\t\t ¦¦¦¦¦¦¦  ¦¦¦¦¦¦  ¦¦   ¦¦  ¦¦¦¦¦¦ ");
		printf("\n\t\t    Sistema de Gestão e Atendimento aos Utentes.");
	}else if(a==2){
		printf("\n\t ¦¦    ¦¦ ¦¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦ ¦¦¦    ¦¦ ¦¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦ ");
		printf("\n\t ¦¦    ¦¦    ¦¦    ¦¦      ¦¦¦¦   ¦¦    ¦¦    ¦¦      ¦¦      ");
		printf("\n\t ¦¦    ¦¦    ¦¦    ¦¦¦¦¦   ¦¦ ¦¦  ¦¦    ¦¦    ¦¦¦¦¦   ¦¦¦¦¦¦¦ ");
		printf("\n\t ¦¦    ¦¦    ¦¦    ¦¦      ¦¦  ¦¦ ¦¦    ¦¦    ¦¦           ¦¦ ");
		printf("\n\t  ¦¦¦¦¦¦     ¦¦    ¦¦¦¦¦¦¦ ¦¦   ¦¦¦¦    ¦¦    ¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦ ");
    	printf("\n\t\t\t\t Menu Utentes");
	}else if(a==3){
		printf("\n\t     ¦¦¦    ¦¦¦ ¦¦¦¦¦¦¦ ¦¦¦¦¦¦  ¦¦  ¦¦¦¦¦¦  ¦¦¦¦¦¦  ¦¦¦¦¦¦¦ ");
		printf("\n\t     ¦¦¦¦  ¦¦¦¦ ¦¦      ¦¦   ¦¦ ¦¦ ¦¦      ¦¦    ¦¦ ¦¦      ");
		printf("\n\t     ¦¦ ¦¦¦¦ ¦¦ ¦¦¦¦¦   ¦¦   ¦¦ ¦¦ ¦¦      ¦¦    ¦¦ ¦¦¦¦¦¦¦ ");
		printf("\n\t     ¦¦  ¦¦  ¦¦ ¦¦      ¦¦   ¦¦ ¦¦ ¦¦      ¦¦    ¦¦      ¦¦ ");
		printf("\n\t     ¦¦      ¦¦ ¦¦¦¦¦¦¦ ¦¦¦¦¦¦  ¦¦  ¦¦¦¦¦¦  ¦¦¦¦¦¦  ¦¦¦¦¦¦¦ ");
    	printf("\n\t\t\t\t Menu Medicos");
	}
	printf("\n");
}

void Menu_Utentes(){
    system("cls");
	int num,num2,i,opcao=1,tecla;
    char** nomes_utentes = Verificar_Medico(&num);
	if(num!=0){
		printf("\nLista de Utentes com Medicos invalidos: ");
		for(i=0;i<num;i++){
			printf("\n%s", nomes_utentes[i]);
		}
		printf("\nEscolha um novo medico para estes utentes!");
		printf("\n\nPressione ENTER para continuar...");
    	getchar(); // aguarda a tecla ENTER ser pressionada
		char** nomes_medicos = Nomes_Medicos("medicos.txt", &num2);
		while (1) {
	    	system("CLS");
	        printf("Lista de medicos disponiveis: \n");
	        for (i = 0; i < num2; i++) {
		        printf("%s  %s\n", opcao == i+1 ? ">": " ", nomes_medicos[i]);
		    }
	
	        tecla = getch();
	
	        if (tecla == 224) {
	            tecla = getch(); 
	
	            switch (tecla) {
	                case 72: 
	                    opcao = opcao == 1 ? num2 : opcao - 1;
	                    break;
	                case 80: 
	                    opcao = opcao == num2 ? 1 : opcao + 1;
	                    break;
	            }
	        } else if (tecla == 13) {
	        	Mudar_Medico_Utentes(nomes_utentes, nomes_medicos[opcao-1], num);
	            break;
	        }
	    }
		for (i = 0; i < num2; i++) {
	        free(nomes_medicos[i]);
	    }
	    free(nomes_medicos);
	}
	for (i = 0; i < num; i++) {
        free(nomes_utentes[i]);
    }
    free(nomes_utentes);

    while (1) {
        system("cls");
        ASCII_Print(2);
        printf("\n\n");
        printf("\t%s  Criar Utente\n", opcao == 1 ? ">": " ");
        printf("\t%s  Editar Utente \n", opcao == 2 ? ">": " ");
        printf("\t%s  Consultar Utente\n", opcao == 3 ? ">": " ");
        printf("\t%s  Remover Utente\n", opcao == 4 ? ">": " ");
        printf("\t%s  Listar Utentes\n", opcao == 5 ? ">": " ");
        printf("\n\t%s  Menu Principal\n", opcao == 6 ? ">": " ");

        tecla = getch();
        if (tecla == 224) { 
            tecla = getch();
            switch (tecla) {
                case 72:
                    opcao = opcao == 1 ? 6 : opcao - 1; 
                    break;
                case 80: 
                    opcao = opcao == 6 ? 1 : opcao + 1;
                    break;
            }
        } else if (tecla == 13) {
            switch(opcao){
            	case 1:
            		Criar_Utente();
            		break;
            	case 2:
            		Editar_Utente();
            		break;
            	case 3:
            		Consultar_Utente();
            		break;
            	case 4:
            		Remover_Utente();
            		break;
            	case 5:
            		Listar_Utentes();
            		break;
            	case 6:
            		Menu_Principal();
            		break;
			}
            break;
        }
    }
}

void Menu_Medicos(){
	int opcao = 1;
    int tecla;

    while (1) {
        system("cls");
        ASCII_Print(3);
        printf("\n\n");
        printf("\t%s  Criar Medico\n", opcao == 1 ? ">": " ");
        printf("\t%s  Editar Medico \n", opcao == 2 ? ">": " ");
        printf("\t%s  Consultar Medico\n", opcao == 3 ? ">": " ");
        printf("\t%s  Remover Medico\n", opcao == 4 ? ">": " ");
        printf("\t%s  Listar Medicos\n", opcao == 5 ? ">": " ");
        printf("\n\t%s  Menu Principal\n", opcao == 6 ? ">": " ");

        tecla = getch();
        if (tecla == 224) { 
            tecla = getch();
            switch (tecla) {
                case 72:
                    opcao = opcao == 1 ? 6 : opcao - 1; 
                    break;
                case 80: 
                    opcao = opcao == 6 ? 1 : opcao + 1;
                    break;
            }
        } else if (tecla == 13) {
            switch(opcao){
            	case 1:
            		Criar_Medico();
            		break;
            	case 2:
            		Editar_Medico();
            		break;
            	case 3:
            		Consultar_Medico();
            		break;
            	case 4:
            		Remover_Medico();
            		break;
            	case 5:
            		Listar_Medicos();
            		break;
            	case 6:
            		Menu_Principal();
            		break;
			}
            break;
        }
    }
}

void Menu_Atendimento(){
	Medico* medico = malloc(sizeof(Medico));
	
	int opcao = 1;
    int tecla;

    while (1) {
        system("cls");
        ASCII_Print(2);
        printf("\n\n");
        printf("\t%s  Colocar na lista de espera\n", opcao == 1 ? ">": " ");
        printf("\t%s  Remover da lista de espera \n", opcao == 2 ? ">": " ");
        printf("\t%s  Ver lista de espera \n", opcao == 3 ? ">": " ");
        printf("\n\t%s  Menu Principal\n", opcao == 4 ? ">": " ");

        tecla = getch();
        if (tecla == 224) { 
            tecla = getch();
            switch (tecla) {
                case 72:
                    opcao = opcao == 1 ? 4 : opcao - 1; 
                    break;
                case 80: 
                    opcao = opcao == 4 ? 1 : opcao + 1;
                    break;
            }
        } else if (tecla == 13) {
            switch(opcao){
            	case 1:
            		Menu_Fila(medico);
            		break;
            	case 2:
            		
            		break;
            	case 3:
            		Menu_Lista_Medico(medico);
            		break;
            	case 4:
            		free(medico);
            		Menu_Principal();
            		break;
			}
            break;
        }
    }
}
