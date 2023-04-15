#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "estruturas.h"
#include "fila.h"

void adicionar_utente(Medico* m, Utente* u) {
    if (m == NULL || u == NULL) {
        return;
    }
    
    if (m->codigo != u->codigo_medico) {
        return;
    }
    
    if (m->fila_espera == NULL) {
        m->fila_espera = u;
        u->proximo = NULL;
        return;
    }
    
    Utente* atual = m->fila_espera;
    while (atual->proximo != NULL) {
        atual = atual->proximo;
    }
    
    atual->proximo = u;
    u->proximo = NULL;
}



Utente* remover_utente_da_fila(Medico* medico) {
    Utente* primeiro_da_fila = medico->fila_espera;
    if (primeiro_da_fila != NULL) {
        medico->fila_espera = primeiro_da_fila->proximo;
        primeiro_da_fila->proximo = NULL;
    }
    return primeiro_da_fila;
}

void listarFilaEsperaMedico(Medico* medico) {
    Utente* utente_atual = medico->fila_espera;
    printf("\nFila de Espera do Medico %s (Codigo: %d):\n", medico->nome, medico->codigo);
    if (utente_atual == NULL) {
        printf("Nao ha pacientes na fila de espera.\n");
        return;
    }
    int posicao = 1;
    while (utente_atual != NULL) {
        printf("%d - %s (Codigo: %d)\n", posicao, utente_atual->nome, utente_atual->codigo);
        utente_atual = utente_atual->proximo;
        posicao++;
    }
}


void Menu_Fila(Medico** medico){
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
						(*medico)->codigo = i;
						strcpy((*medico)->nome, linha);
						adicionar_utente(*medico, &utente);
						//listarFilaEsperaMedico(*medico);
						fclose(arquivo);
						fclose(arquivo2);
					}
				}
			}
		}
		fclose(arquivo);
		fclose(arquivo2);
		if (*medico != NULL) {
			listarFilaEsperaMedico(*medico);
		}
		getch();
		Menu_Atendimento();
	} else {
		printf("Utente nao encontrado!");
		getch();
		Menu_Atendimento();
	}
}

void Menu_Lista_Medico(Medico** medico) {
    system("CLS");
    int num, i, opcao = 0, tecla;
    char** nomes_medicos = Nomes_Medicos("medicos.txt", &num);

    if (num == 0) {
        printf("Não há médicos registrados! Registre um médico primeiro.");
        getch();
        Menu_Utentes();
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
                case 72: // seta para cima
                    opcao = (opcao + num - 1) % num;
                    break;
                case 80: // seta para baixo
                    opcao = (opcao + 1) % num;
                    break;
            }
        } else if (tecla == 13) { // enter
            strcpy((*medico)->nome, nomes_medicos[opcao]);
            (*medico)->codigo = Codigo_Medico(nomes_medicos[opcao]);
            listarFilaEsperaMedico(medico);
            break;
        }
    }

    for (i = 0; i < num; i++) {
        free(nomes_medicos[i]);
    }
    free(nomes_medicos);

    getch();
    Menu_Utentes();
}

