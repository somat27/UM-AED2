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

void voltar_menu_fila(){
	printf("\n\nPressione ENTER para continuar...");
	getchar(); // aguarda a tecla ENTER ser pressionada
	Menu_Atendimento();
}


//Função retona o médico com mais utentes em fila de espera
Medico* Encontrar_Medico_Com_Mais_Utentes_Espera(int *qtdutentes) {
    Medico* atualMedico = medicos;
    Medico* medicoComMaisUtentesEspera = NULL;
    int maxUtentesEspera = -1;
	if(atualMedico == NULL){
		return NULL;	//verifica se existem utentes na fila
	}
    while (atualMedico != NULL) {
        Utente* atualUtente = atualMedico->fila_espera;
        int numUtentesEspera = 0;
        while (atualUtente != NULL) {	//lopp de forma a contar
            numUtentesEspera++;
            atualUtente = atualUtente->proximo;
        }
        //verifica se há um novo médico com um máximo de utentes em fila de espera
        if (numUtentesEspera > maxUtentesEspera) { 
            maxUtentesEspera = numUtentesEspera;
            medicoComMaisUtentesEspera = atualMedico;
        }
        atualMedico = atualMedico->proximo;
    }
    *qtdutentes = maxUtentesEspera;		
    return medicoComMaisUtentesEspera;
}


//Função que verfica se o utente está em fila de espera
int Utente_Esta_Na_Fila_De_Espera(int codigoUtente) {
    Medico* atualMedico = medicos;
    while (atualMedico != NULL) {
        Utente* atualUtente = atualMedico->fila_espera;
        //procura na fila se o utente já lá está inserido
        while (atualUtente != NULL) {
            if (atualUtente->codigo == codigoUtente) {
                return 1;
            }
            atualUtente = atualUtente->proximo;
        }
        atualMedico = atualMedico->proximo;
    }
    return 0;
}

int Verificar_Fila_Espera_Medico(int codigoMedico) {
    Medico* atualMedico = medicos;
    while (atualMedico != NULL) {
        if (atualMedico->codigo == codigoMedico) {
            Utente* atualUtente = atualMedico->fila_espera;
            if (atualUtente == NULL) {
                return 0; // não há utentes na fila de espera do médico
            } else {
                return 1; // há utentes na fila de espera do médico
            }
        }
        atualMedico = atualMedico->proximo;
    }
    return -1; // o médico com o código fornecido não foi encontrado
}

void Menu_Atendimento(){
	int opcao = 1;
    int tecla;

    while (1) {
        system("cls");
        ASCII_Print(2);
        printf("\n\n");
        int qtdutentes;
        Medico* medico_mais_ocupado = Encontrar_Medico_Com_Mais_Utentes_Espera(&qtdutentes);
        if(medico_mais_ocupado != NULL && qtdutentes != 0){
        	printf("\t%s tem %d utentes em espera formando a maior fila de espera!\n\n",medico_mais_ocupado->nome,qtdutentes);
		}
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

void Criar_Medico_Apontador(int i, char *linha){
	int medico_ja_criado = 0;
	
	Medico* atualMedico = medicos;
    while (atualMedico != NULL) {
        if (atualMedico->codigo == i) {
            medico_ja_criado = 1;
            return;
        }
        atualMedico = atualMedico->proximo;
    }
	if(medico_ja_criado == 0){
		// Cria o novo médico
	    Medico* novoMedico = malloc(sizeof(Medico));
	    novoMedico->codigo = i;
	    strcpy(novoMedico->nome, linha);
	    novoMedico->fila_espera = NULL;
	    
	    // Adiciona o novo médico à lista
	    novoMedico->proximo = medicos;
	    medicos = novoMedico;
	}
}

void Colocar_Utente_Fila_Espera(char *string, Utente *utente){
	Utente* novoUtenteFila = malloc(sizeof(Utente));
	novoUtenteFila->codigo = utente->codigo;
	novoUtenteFila->codigo_medico = utente->codigo_medico;
	strcpy(novoUtenteFila->nome, utente->nome);
	novoUtenteFila->proximo = NULL;	
	
	Medico* atualMedico = medicos;
	while (atualMedico != NULL) {
	    if (atualMedico->codigo == utente->codigo_medico) {
	        Utente* atualUtente = atualMedico->fila_espera;
	        /*Caso seja o primeiro utente é adicionado diretamente á fila de espera*/
	        if (atualUtente == NULL) { 
	            atualMedico->fila_espera = novoUtenteFila;
				printf("\n%s foi adicionado a lista de espera de %s\n", string, atualMedico->nome);
	        } else {
	            while (atualUtente != NULL) {	//verfica se o utente já esta na fila
				    if (atualUtente->codigo == novoUtenteFila->codigo) {
				        printf("\nEsse utente ja se encontra na fila de espera!");
				        break;
				    }
				    /*Caso não esteja e a fila de espera ja tenha um utente, 
					adiciona-o de forma a ficar na próxima posição da fila */
				    if (atualUtente->proximo == NULL) {	
				        atualUtente->proximo = novoUtenteFila;
				        printf("\n%s foi adicionado a lista de espera de %s\n", string, atualMedico->nome);	
				        break;
				    }
				    atualUtente = atualUtente->proximo;
				}

	        }				        
	        break;
	    }
	    atualMedico = atualMedico->proximo;
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
		
		//verifica se o utente procurado está nos files
		while (fscanf(arquivo, "%[^,],%d,%d\n", utente.nome, &utente.codigo, &utente.codigo_medico) == 3) {
			if (strcmp(string, utente.nome) == 0) {
				//caso esteja procura o seu médico
				while (fscanf(arquivo2, "%[^,],%d\n", linha, &i) == 2) {
					if (i == utente.codigo_medico) { //quando encontrar adiciona á fila de espera
						Criar_Medico_Apontador(i, linha);
						Colocar_Utente_Fila_Espera(string, &utente);	
						break;
					}
				}
			}
		}
		fclose(arquivo);
		fclose(arquivo2);
		voltar_menu_fila();
	} else {
		printf("Utente nao encontrado!");
		voltar_menu_fila();
	}
}

void Menu_Lista_Medico() {
    system("CLS");
    int num, i, opcao = 0, tecla;
    char** nomes_medicos = Nomes_Medicos("medicos.txt", &num);

    if (num == 0) {	//verfica se já existem médicos registados
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
        	system("CLS");
            int codigo_medico = Codigo_Medico(nomes_medicos[opcao]);
            int qtdUtentes = 0;
            Medico* atualMedico = medicos;
			while (atualMedico != NULL) {
			    if(atualMedico->codigo == codigo_medico){
        			printf("Lista de espera do medico: %s\n\n", atualMedico->nome);
			    	Utente* atualUtente = atualMedico->fila_espera;
			    	//Lista todos os utentes organizados por médico
				    while (atualUtente != NULL) {
				        printf("\nNome: %s, Código: %d", atualUtente->nome, atualUtente->codigo);
				        atualUtente = atualUtente->proximo;
				        qtdUtentes++;
				    }
				}
			    atualMedico = atualMedico->proximo;
			}     
			if(qtdUtentes == 0){
				printf("\nNao existe ninguem na fila de espera!");
			}else{
				printf("\n\nTotal de %d utentes em fila de espera", qtdUtentes);	
			}
			  
            break;
        }
    }

    for (i = 0; i < num; i++) {
        free(nomes_medicos[i]);
    }
    free(nomes_medicos);
	voltar_menu_fila();
}

void Remover_Primeiro_Utente_Fila_Espera(Medico *medico) {
    Utente *primeiroUtente = medico->fila_espera;
    if (primeiroUtente == NULL) {
        printf("Fila de espera vazia.\n");
        return;
    }
    medico->fila_espera = primeiroUtente->proximo;
    //liberta a primeira posição da fila
    free(primeiroUtente);
    printf("Primeiro utente da fila de espera de %s removido.\n", medico->nome);
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
        	int codigoMedico = Codigo_Medico(nomes_medicos[opcao]);
        	
            Medico* atualMedico = medicos;
		    while (atualMedico != NULL) {
		        if (atualMedico->codigo == codigoMedico) {
		        	if (atualMedico->fila_espera == NULL) {
		        		//verifica se a fila de espera não está vazia
				        printf("Erro: Nao ha ninguem na fila de espera!\n");
				    }else{
				    	//caso não esteja remove o primeiro utente da fila de espera
		        		Remover_Primeiro_Utente_Fila_Espera(atualMedico);
					}
		            break;
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
	voltar_menu_fila();
}
