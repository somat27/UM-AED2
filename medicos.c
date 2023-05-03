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

void voltar_menu_medicos(){
	printf("\n\nPressione ENTER para continuar...");
	getchar(); // aguarda a tecla ENTER ser pressionada
	Menu_Medicos();
}

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
		voltar_menu_medicos();
	}
	int codigo = Gerar_Codigo();	
	Medico medico;
	strcpy(medico.nome, nome);
    medico.codigo = codigo;
    guardarBaseDadosMedico(&medico);
    printf("Medico criado com sucesso!");
	voltar_menu_medicos();
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
		        	int codigo_utente = Codigo_Utente(nome_utente);
					if(Utente_Esta_Na_Fila_De_Espera(codigo_utente) == 1){
						printf(" - Este utente esta na fila de espera!");
					}else{
						printf(" - Este utente nao esta na fila de espera!");
					}
				}
		    }
		    fseek(arquivo_utentes, 0, SEEK_SET); // volta ao início do arquivo de utentes
		}
    }
    fclose(arquivo_medicos);
    fclose(arquivo_utentes);
    
	voltar_menu_medicos();
}

void Remover_Medico(){
	system("CLS");
	char nome[255];
	printf("Nome do Medico de deseja remover: ");
	gets(nome);
	
	int cod_medico = Codigo_Medico(nome);
	if(Verificar_Fila_Espera_Medico(cod_medico) == 1){
		printf("\nEste medico tem utentes na fila de espera. Remova-os antes de remover este medico!");
    	voltar_menu_medicos();
	}
	
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
    
	voltar_menu_medicos();
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
	voltar_menu_medicos();
}

void Editar_Medico(){
	system("CLS");
	char string[255], string2[255];
	int encontrou = 0;
	printf("Nome do Medico que quer editar: ");
	fflush(stdin);
	gets(string);
	
	int cod_medico = Codigo_Medico(string);
	if(Verificar_Fila_Espera_Medico(cod_medico) == 1){
		printf("\nEste medico tem utentes na fila de espera. Remova-os antes de remover este medico!");
    	voltar_menu_medicos();
	}
	
    FILE* arquivo = fopen("medicos.txt", "r");
    FILE* arquivoTemp = fopen("temp.txt", "w");
    Medico medico;
    while (fscanf(arquivo, "%[^,],%d\n", medico.nome, &medico.codigo) == 2)
    {
        if (strcmp(medico.nome, string) == 0)
        {
        	encontrou = 1;
			printf("Novo Nome: ");
			fflush(stdin);
			gets(string2);
			strcpy(medico.nome, string2);
		    fprintf(arquivoTemp, "%s,%d\n", medico.nome, medico.codigo);
        }
        else
        {
            fprintf(arquivoTemp, "%s,%d\n", medico.nome, medico.codigo);
        }
    }
    fclose(arquivo);
    fclose(arquivoTemp);
    remove("medicos.txt");
    rename("temp.txt", "medicos.txt");
    
    if(encontrou == 0){
    	printf("\nNao foi possivel encontrar esse Medico!");
	}else{
    	printf("\nUtente editado com sucesso!");
	}
		voltar_menu_medicos();
}
