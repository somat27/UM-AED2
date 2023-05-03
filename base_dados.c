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

void guardarBaseDadosUtente(Utente* utente){
    FILE* arquivo = fopen("utentes.txt", "a");
    fprintf(arquivo, "%s,%d,%d\n", utente->nome,utente->codigo,utente->codigo_medico);
    fclose(arquivo);
}

int RemoverUtente(char* nomeUtente){
	int a = 0;
    FILE* arquivo = fopen("utentes.txt", "r");
    FILE* arquivoTemp = fopen("temp.txt", "w");

    Utente utente;
	while (fscanf(arquivo, "%[^,],%d,%d\n", utente.nome, &utente.codigo, &utente.codigo_medico) == 3) //O [^,] ï¿½ para ler uma string ate a proxima ','
    {
    	/* 
		Caso utente.nome seja diferente do nomeUtente entao copiamos as informações para 
		o ficheiro temporario, se eles forem iguais entao nao copia para o ficheiro temporario
		*/
    	if (strcmp(utente.nome, nomeUtente) != 0){
            fprintf(arquivoTemp, "%s,%d,%d\n", utente.nome, utente.codigo, utente.codigo_medico);
        }else{
        	a = 1;
		}
    }

    fclose(arquivo);
    fclose(arquivoTemp);
    
    /* 
	Agora eliminamos o ficheiro utentes.txt que continha os utentes antigos e renomeamos
	o ficheiro temporario para ser o novo utentes.txt com as alterações feitas
	*/
    
	remove("utentes.txt");
	rename("temp.txt", "utentes.txt");
	return a;
}

void guardarBaseDadosMedico(Medico* medico){
    FILE* arquivo = fopen("medicos.txt", "a");
    fprintf(arquivo, "%s,%d\n", medico->nome,medico->codigo);
    fclose(arquivo);
}

int RemoverMedico(char* nomeMedico){
	int a = 0;
    FILE* arquivo = fopen("medicos.txt", "r");
    FILE* arquivoTemp = fopen("temp.txt", "w");

    Medico medico;
	while (fscanf(arquivo, "%[^,],%d\n", medico.nome, &medico.codigo) == 2) //O [^,] ï¿½ para ler uma string ate a proxima ','
    {
    	if (strcmp(medico.nome, nomeMedico) != 0)
        {
            fprintf(arquivoTemp, "%s,%d\n", medico.nome, medico.codigo);
        }else{
        	a = 1;
		}
    }

    fclose(arquivo);
    fclose(arquivoTemp);
    
	remove("medicos.txt");
	rename("temp.txt", "medicos.txt");
	return a;
}

int Gerar_Codigo(){
    srand(time(NULL));

    int num_aleatorio, encontrou = 0;
    char str_num[7];
    
    FILE* arquivo = fopen("utentes.txt", "r");
    char linha[100];

    do {
        encontrou = 0;
        /*
		Gera um numero aleatorio entre 0 e 9999999 mas como eu quero numero superiores a 1000000
		temos de somar 100000 para garantir que o numero gerado esta dentro do intervalo desejado
		*/
        num_aleatorio = rand() % 999999 + 100000; 
        /*
		Verificação para saber se este codigo ja esta a ser ou nao utilizado noutras contas
		*/
        while (fgets(linha, sizeof(linha), arquivo)) {
        	char* nome_utente = strtok(linha, ",");
	        char* codigo_utente = strtok(NULL, ",");
	        char* codigo_medico = strtok(NULL, "\n");
			int cod_utente = atoi(codigo_utente);
			int cod_medico = atoi(codigo_medico);
	        if (cod_utente == num_aleatorio || cod_medico == num_aleatorio) {
	            encontrou = 1;
	            break;
	        }
	    }

        if(encontrou == 0){
	    	fclose(arquivo);
	    	return num_aleatorio;
		}

    } while (1);
}

void Mudar_Medico_Utentes(char** nomes_utentes, char* nome_medico, int num_utentes){
	int i, encontrou;
	
    FILE* arquivo = fopen("medicos.txt", "r");
    FILE* arquivo2 = fopen("utentes.txt", "r");
    FILE* arquivoTemp = fopen("temp.txt", "w");	
    
    char linha[100],linha2[100];
    int cod_medico;
    Utente utente;
    
    while (fgets(linha, 100, arquivo) != NULL) {
        char* string = strtok(linha, ",");
        char* codigo = strtok(NULL, "\n");
        if(strcmp(string, nome_medico) == 0){
			cod_medico = atoi(codigo);
        	break;
		}
    }
    fclose(arquivo);
    
    while (fgets(linha2, 100, arquivo2) != NULL) {
    	encontrou = 0;
        char* nome_utente = strtok(linha2, ",");
        char* codigo_utente = strtok(NULL, ",");
        char* codigo_medico_utente = strtok(NULL, "\n");
        int cod_utente = atoi(codigo_utente);
		int cod_medico_utente = atoi(codigo_medico_utente);
		strcpy(utente.nome, nome_utente);
		utente.codigo = cod_utente;
		for(i=0;i<num_utentes;i++){
			if(strcmp(nomes_utentes[i], nome_utente) == 0){
				encontrou = 1;
			}
		}
		if(encontrou){
			utente.codigo_medico = cod_medico;
			fprintf(arquivoTemp, "%s,%d,%d\n", utente.nome, utente.codigo, utente.codigo_medico);
		}else{
			utente.codigo_medico = cod_medico_utente;
			fprintf(arquivoTemp, "%s,%d,%d\n", utente.nome, utente.codigo, utente.codigo_medico);
		}
		
    }
    
    fclose(arquivo2);
    fclose(arquivoTemp);
    remove("utentes.txt");
    rename("temp.txt", "utentes.txt");
}
