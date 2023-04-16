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

char** Nomes_Medicos(char* nome_arquivo, int* n_medicos){
    FILE* arquivo = fopen("medicos.txt", "r");
    
    char** nomes = (char**) malloc(sizeof(char*));
    if (nomes == NULL) {
        printf("Erro ao alocar memoria\n");
        return NULL;
    }
    
    char linha[100];
    int i = 0;
    while (fgets(linha, 100, arquivo) != NULL) {
        char* nome = strtok(linha, ",");
        char* codigo_str = strtok(NULL, ",");
        
        nomes[i] = (char*) malloc(sizeof(char));
        if (nomes[i] == NULL) {
            printf("Erro ao alocar memoria\n");
            return NULL;
        }
        strcpy(nomes[i], nome);
        i++;
    }
    *n_medicos = i;
    
    fclose(arquivo);
    return nomes;
}

int Codigo_Medico(char* Nome_Medico){
	FILE* arquivo = fopen("medicos.txt", "r");
    
	char linha[100];
	char *num_str;
    while (fgets(linha, 100, arquivo) != NULL) {
        char* nome = strtok(linha, ",");
        char* codigo_str = strtok(NULL, ",");
        
        if(strcmp(nome ,Nome_Medico) == 0){
		    num_str = strtok(codigo_str, "\n");// remove o \n do char
		    int num_int = atoi(num_str);// converte o char para int
        	return num_int;
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
	gets(nome);
	if(VerificarNomeMedico(nome) == 1){
		printf("Ja existe um medico chamado \"%s\"",nome);
		getch();
		Menu_Medicos();
	}
	int codigo = Gerar_Codigo();	
	Medico medico;
	strcpy(medico.nome, nome);
    medico.codigo = codigo;
    guardarBaseDadosMedico(&medico);
    printf("Medico criado com sucesso!");
    getch();
    Menu_Medicos();
}

void Consultar_Medico(){
	system("CLS");
	char string[255];
	printf("Nome do Medico: ");
	gets(string);
	char linha[100],linha2[100];
    int i = 0;
    FILE* arquivo = fopen("medicos.txt", "r");
    FILE* arquivo2 = fopen("utentes.txt", "r");
    while (fgets(linha, 100, arquivo) != NULL) {
        char* nome_medico = strtok(linha, ",");
        char* codigo_medico = strtok(NULL, "\n");
		int cod_medico = atoi(codigo_medico);
        if(strcmp(string, nome_medico)==0){
        	printf("\n\nMedico: %s, Codigo: %d\nLista de Utentes:", nome_medico, cod_medico);
		    while (fgets(linha2, 100, arquivo2) != NULL) {
		        char* nome_utente = strtok(linha2, ",");
		        char* codigo_utente = strtok(NULL, ",");
		        char* codigo_medico_utente = strtok(NULL, "\n");
				int cod_utente = atoi(codigo_utente);
				int cod_medico_utente = atoi(codigo_medico_utente);
		        if(cod_medico == cod_medico_utente){
		        	printf("\n Nome: %s, Codigo: %d", nome_utente, cod_utente);
				}
		    }
		}
    }
    fclose(arquivo);
    getch();
    Menu_Medicos();
}

void Remover_Medico(){
	system("CLS");
	char nome[255];
	printf("Nome do Medico de deseja remover: ");
	gets(nome);
	if(RemoverMedico(nome)){
		Menu_Medicos();
	}else{
		printf("Nao foi encontrado esse nome!");		
	}
}

void Listar_Medicos(){
	system("CLS");
	printf("Lista de Medicos registados: ");
	char linha[100];
    int i = 0;
    FILE* arquivo = fopen("medicos.txt", "r");
    while (fgets(linha, 100, arquivo) != NULL) {
        char* nome = strtok(linha, ",");
        char* codigo_str = strtok(NULL, "\n");
		int num_int = atoi(codigo_str);
        printf("\n\nNome: %s, Codigo: %d", nome, num_int);
    }
    fclose(arquivo);
    getch();
    Menu_Medicos();
}
