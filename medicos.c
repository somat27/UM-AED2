#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "estruturas.h"
#include "medicos.h"

int VerificarNomeMedico(char* nome){	
    FILE* arquivo = fopen("medicos.txt", "r");
    Medico medico;
    while (fscanf(arquivo, "%[^,],%d\n", medico.nome, &medico.codigo) == 2)
    {
        if (strcmp(medico.nome, nome) == 0)
        {
    		fclose(arquivo);
        	return 1;
        }
    }
    fclose(arquivo);
    return 0;	
}

char** Nomes_Medicos(char* nome_arquivo, int* n_medicos) {
    FILE* arquivo = fopen("medicos.txt", "r");
    
    char** nomes = NULL;
    char linha[100];
    int i = 0;
    while (fgets(linha, 100, arquivo) != NULL) {
        char* nome = strtok(linha, ",");
        nomes = realloc(nomes, (i + 1) * sizeof(char*));
        nomes[i] = strdup(nome);//duplica a string de nome para nomes[i]
        i++;
    }
    *n_medicos = i;

    fclose(arquivo);
    return nomes;
}

int Codigo_Medico(char* Nome_Medico) {
    FILE* arquivo = fopen("medicos.txt", "r");

    char linha[100];
    while (fgets(linha, 100, arquivo) != NULL) {
        char* nome_medico = strtok(linha, ",");
        char* codigo_medico = strtok(NULL, "\n");
        if (strcmp(nome_medico, Nome_Medico) == 0) {
            int cod_medico = atoi(codigo_medico);
            fclose(arquivo);
            return cod_medico;
        }
    }
    fclose(arquivo);
    return -1;
}


void Criar_Medico(){
	system("CLS");
	char nome[255];
	printf("Registo de Medico: \n");
	printf("Nome do Medico: ");
	fgets(nome, 255, stdin);
	nome[strcspn(nome, "\n")] = '\0'; // remove a nova linha
	if(VerificarNomeMedico(nome) == 1){
		printf("Ja existe um medico chamado \"%s\"",nome);
		printf("\n\nPressione ENTER para continuar...");
    	getchar(); // aguarda a tecla ENTER ser pressionada
		Menu_Medicos();
	}
	int codigo = Gerar_Codigo();	
	Medico medico;
	strcpy(medico.nome, nome);
    medico.codigo = codigo;
    guardarBaseDadosMedico(&medico);
    printf("Medico criado com sucesso!");
    printf("\n\nPressione ENTER para continuar...");
    getchar(); // aguarda a tecla ENTER ser pressionada
    Menu_Medicos();
}

void Consultar_Medico(){
	system("CLS");
	
	char nome_medico[255];
	printf("Nome do Medico: ");
	fgets(nome_medico, 255, stdin);
	nome_medico[strcspn(nome_medico, "\n")] = '\0'; // remove o caractere de nova linha da string

	FILE* arquivo_medicos = fopen("medicos.txt", "r");	
	FILE* arquivo_utentes = fopen("utentes.txt", "r");

	char linha_medico[255];
    while (fgets(linha_medico, 255, arquivo_medicos) != NULL) {
        char* nome_medico_arquivo = strtok(linha_medico, ",");
        char* codigo_medico_arquivo = strtok(NULL, "\n");
		int cod_medico_arquivo = atoi(codigo_medico_arquivo);
		
        if(strcmp(nome_medico, nome_medico_arquivo) == 0) {
        	printf("\n\nMedico: %s, Codigo: %d\nLista de Utentes:", nome_medico_arquivo, cod_medico_arquivo);
        	
        	char linha_utente[255];
		    while (fgets(linha_utente, 255, arquivo_utentes) != NULL) {
		        char* nome_utente = strtok(linha_utente, ",");
		        char* codigo_utente = strtok(NULL, ",");
		        char* codigo_medico_utente = strtok(NULL, "\n");
				int cod_utente = atoi(codigo_utente);
				int cod_medico_utente = atoi(codigo_medico_utente);
		        
		        if(cod_medico_arquivo == cod_medico_utente) {
		        	printf("\n Nome: %s, Codigo: %d", nome_utente, cod_utente);
				}
		    }
		    fseek(arquivo_utentes, 0, SEEK_SET); // volta ao início do arquivo de utentes
		}
    }
    fclose(arquivo_medicos);
    fclose(arquivo_utentes);
    
    printf("\n\nPressione ENTER para continuar...");
    getchar(); // aguarda a tecla ENTER ser pressionada
    Menu_Medicos();
}

void Remover_Medico(){
	system("CLS");
	char nome[255];
	printf("Nome do Medico de deseja remover: ");
	gets(nome);
	
	printf("Tem certeza que deseja remover o medico %s? (S/N)\n", nome);
    char resposta = getchar();
    getchar(); // Limpar o buffer de entrada
    
	if (resposta == 'S' || resposta == 's') {
		if(RemoverMedico(nome)){
			printf("O medico foi removido com sucesso!\n");
		}else{
			printf("Nao foi encontrado esse nome!");		
		}
	} else {
        printf("Operacao cancelada pelo usuario!\n");
    }
    
    printf("\n\nPressione ENTER para continuar...");
    getchar(); // aguarda a tecla ENTER ser pressionada
    Menu_Medicos();
}

void Listar_Medicos(){
	system("CLS");
	printf("Lista de Medicos registados: \n");
	char linha[100];
    int i = 0;
    FILE* arquivo = fopen("medicos.txt", "r");
    while (fgets(linha, 100, arquivo) != NULL) {
        char* nome = strtok(linha, ",");
        char* codigo_str = strtok(NULL, "\n");
		int num_int = atoi(codigo_str);
        printf("\nNome: %s, Codigo: %d", nome, num_int);
    }
    fclose(arquivo);
    printf("\n\nPressione ENTER para continuar...");
    getchar(); // aguarda a tecla ENTER ser pressionada
    Menu_Medicos();
}
