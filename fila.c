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

Medico *medicos = NULL;

void Menu_Atendimento(){
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
            		Menu_Fila();
            		break;
            	case 2:
            		Menu_Remover_Lista_Medico();
            		break;
            	case 3:
            		Menu_Lista_Medico();
            		break;
            	case 4:
            		Menu_Principal();
            		break;
			}
            break;
        }
    }
}

void Menu_Fila(){
	system("CLS");
	ASCII_Print(2);
	char string[255];
	Utente utente;
	printf("Nome do utente para entrar na fila de espera: ");
	fgets(string, sizeof(string), stdin);
	string[strcspn(string, "\n")] = '\0'; // remove o caractere '\n' do final da string
	if (VerificarNomeUtente(string) == 1) {
		char linha[100];
		int i = 0;
		FILE* arquivo = fopen("utentes.txt", "r");
		FILE* arquivo2 = fopen("medicos.txt", "r");
		while (fscanf(arquivo, "%[^,],%d,%d\n", utente.nome, &utente.codigo, &utente.codigo_medico) == 3) {
			if (strcmp(string, utente.nome) == 0) {
				while (fscanf(arquivo2, "%[^,],%d\n", linha, &i) == 2) {
					if (i == utente.codigo_medico) {
						
						Medico* novoMedico = malloc(sizeof(Medico));
						novoMedico->codigo = i;
						strcpy(novoMedico->nome, linha);
						novoMedico->fila_espera = NULL;
						novoMedico->proximo = medicos;
						medicos = novoMedico;
						
						Utente* novoUtenteFila = malloc(sizeof(Utente));
						novoUtenteFila->codigo = utente.codigo;
						strcpy(novoUtenteFila->nome, utente.nome);
						novoUtenteFila->proximo = NULL;	
						
						Medico* atualMedico = medicos;
						while (atualMedico != NULL) {
						    if (atualMedico->codigo == utente.codigo_medico) {
						        Utente* atualUtente = atualMedico->fila_espera;
						        if (atualUtente == NULL) {
						            atualMedico->fila_espera = novoUtenteFila;
						        } else {
						            while (atualUtente->proximo != NULL) {
						                atualUtente = atualUtente->proximo;
						            }
						            atualUtente->proximo = novoUtenteFila;
						        }
						        
						        printf("\n%s foi adicionado a lista de espera de %s\n", string, atualMedico->nome);
						        
						        break;
						    }
						    atualMedico = atualMedico->proximo;
						}	
											
						break;
					}
				}
			}
		}
		fclose(arquivo);
		fclose(arquivo2);
		printf("\n\nPressione ENTER para continuar...");
    	getchar(); // aguarda a tecla ENTER ser pressionada
		Menu_Atendimento();
	} else {
		printf("Utente nao encontrado!");
		printf("\n\nPressione ENTER para continuar...");
    	getchar(); // aguarda a tecla ENTER ser pressionada
		Menu_Atendimento();
	}
}

void Menu_Lista_Medico() {
    system("CLS");
    int num, i, opcao = 0, tecla;
    char** nomes_medicos = Nomes_Medicos("medicos.txt", &num);

    if (num == 0) {
        printf("Não há médicos registrados! Registre um médico primeiro.");
        printf("\n\nPressione ENTER para continuar...");
    	getchar();
        Menu_Atendimento();
    }

    while (1) {
        system("CLS");
        printf("Lista de Médicos:\n\n");
        for (i = 0; i < num; i++) {
            printf("%s %s\n", opcao == i ? ">" : " ", nomes_medicos[i]);
        }

        tecla = getch();

        if (tecla == 224) {
            tecla = getch(); 

            switch (tecla) {
                case 72:
                    opcao = (opcao + num - 1) % num;
                    break;
                case 80:
                    opcao = (opcao + 1) % num;
                    break;
            }
        } else if (tecla == 13) {
            int codigo_medico = Codigo_Medico(nomes_medicos[opcao]);
            
            Medico* atualMedico = medicos;
			while (atualMedico != NULL) {
			    if(atualMedico->codigo == codigo_medico){
			    	Utente* atualUtente = atualMedico->fila_espera;
				    while (atualUtente != NULL) {
				        printf("Nome: %s, Código: %d\n", atualUtente->nome, atualUtente->codigo);
				        atualUtente = atualUtente->proximo;
				    }
				}
			    atualMedico = atualMedico->proximo;
			}     
			  
            break;
        }
    }

    for (i = 0; i < num; i++) {
        free(nomes_medicos[i]);
    }
    free(nomes_medicos);

    printf("\n\nPressione ENTER para continuar...");
    getchar();
    Menu_Atendimento();
}

void Menu_Remover_Lista_Medico() {
    system("CLS");
    int num, i, opcao = 0, tecla;
    char** nomes_medicos = Nomes_Medicos("medicos.txt", &num);

    if (num == 0) {
        printf("Não há médicos registrados! Registre um médico primeiro.");
        printf("\n\nPressione ENTER para continuar...");
    	getchar();
        Menu_Atendimento();
    }

    while (1) {
        system("CLS");
        printf("Lista de Médicos:\n\n");
        for (i = 0; i < num; i++) {
            printf("%s %s\n", opcao == i ? ">" : " ", nomes_medicos[i]);
        }

        tecla = getch();

        if (tecla == 224) {
            tecla = getch(); 

            switch (tecla) {
                case 72:
                    opcao = (opcao + num - 1) % num;
                    break;
                case 80:
                    opcao = (opcao + 1) % num;
                    break;
            }
        } else if (tecla == 13) {
            int codigo_medico = Codigo_Medico(nomes_medicos[opcao]);
            
            Medico* atualMedico = medicos;
			while (atualMedico != NULL) {
			    if(atualMedico->codigo == codigo_medico){
			    	Utente* atualUtente = atualMedico->fila_espera;
				    while (atualUtente != NULL) {
				        printf("Nome: %s, Código: %d\n", atualUtente->nome, atualUtente->codigo);
				        atualUtente = atualUtente->proximo;
				    }
				}
			    atualMedico = atualMedico->proximo;
			}     
			  
			printf("\n\nTem a certeza que deseja remover o ultimo utente? (S/N)");
		    char resposta = getchar();
		    getchar();
		    
			if (resposta == 'S' || resposta == 's') {
				Medico* atualMedico = medicos;
			    while (atualMedico != NULL) {
			        if (atualMedico->codigo == codigo_medico) {
			            Utente* atual = atualMedico;			   
						while (atual->proximo->proximo != NULL) {
					        atual = atual->proximo;
					    }       
						
						Utente* ultimo = atual->proximo;
    					atual->proximo = NULL;
						  
						free(ultimo);

			            break;
			        }
			        atualMedico = atualMedico->proximo;
			    }
				printf("\nO utente foi removido com sucesso!");
			} else {
		        printf("\nOperacao cancelada pelo usuario!");
		    }  
			
            break;
        }
    }
	
    for (i = 0; i < num; i++) {
        free(nomes_medicos[i]);
    }
    free(nomes_medicos);

    printf("\n\nPressione ENTER para continuar...");
    getchar(); // aguarda a tecla ENTER ser pressionada
    Menu_Atendimento();
}
