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

void voltar_menu_utentes(){
	printf("\n\nPressione ENTER para continuar...");
	getchar(); // aguarda a tecla ENTER ser pressionada
	Menu_Utentes();
}

int VerificarNomeUtente(char* nome){	//percorre o ficheiro para verificar se aquele nome já existe
    FILE* arquivo = fopen("utentes.txt", "r");
    Utente utente;
    while (fscanf(arquivo, "%[^,],%d,%d\n", utente.nome, &utente.codigo, &utente.codigo_medico) == 3)
    {
        if (strcmp(utente.nome, nome) == 0)
        {
    		fclose(arquivo);
        	return 1;
        }
    }
    fclose(arquivo);
    return 0;	
}

void Criar_Utente(){
	system("CLS");
	char nome[255],medico[255];
	int codigo = Gerar_Codigo();
	int num,i;
    int opcao = 1;
    int tecla;
	
		/*Nomes_Medicos lê o arquivo "medicos.txt" 
		e retorna um array de strings contendo os nomes 
		dos médicos encontrados no arquivo.*/
	char** nomes_medicos = Nomes_Medicos("medicos.txt",&num); 
	if(num == 0){
		printf("Não há medicos registados!\nRegiste um medico primeiro!");
		voltar_menu_utentes();
	} 
	
	printf("Registo de Utente: \n");
	printf("Nome do Utente: ");
	gets(nome);
	if(VerificarNomeUtente(nome) == 1){
		printf("Ja existe um utente chamado \"%s\"",nome);
		voltar_menu_utentes();
	}

	Utente utente;
    while (1) {
    	system("CLS");
        printf("Qual é o medico de %s\n\n",nome);
        for (i = 0; i < num; i++) {					//Listar Médicos 
	        printf("%s  %s\n", opcao == i+1 ? ">": " ", nomes_medicos[i]);
	    }

        tecla = getch();

        if (tecla == 224) {
            tecla = getch(); 

            switch (tecla) {
                case 72: 
                    opcao = opcao == 1 ? num : opcao - 1;
                    break;
                case 80: 
                    opcao = opcao == num ? 1 : opcao + 1;
                    break;
            }
        } else if (tecla == 13) {
        	strcpy(medico, nomes_medicos[opcao-1]);
        	int cod_medico = Codigo_Medico(medico);
		    
		    strcpy(utente.nome, nome);
		    utente.codigo = codigo;
		    utente.codigo_medico = cod_medico;
		    guardarBaseDadosUtente(&utente);
		    
		    Criar_Medico_Apontador(cod_medico, medico);
		    Colocar_Utente_Fila_Espera(utente.nome, &utente);
		    
            break;
        }
    }
	for (i = 0; i < num; i++) {
        free(nomes_medicos[i]);
    }
    free(nomes_medicos);
    voltar_menu_utentes();
}

void Consultar_Utente(){
	system("CLS");
	char string[255];
	printf("Nome do Utente: ");
	gets(string);
	char linha[100],linha2[100];
    int i = 0;
    FILE* arquivo = fopen("utentes.txt", "r");
	FILE* arquivo2 = fopen("medicos.txt", "r");
	
	/* lê cada linha do arquivo um por um e armazena cada 
	parte da linha como uma string usando a função "strtok"*/
    while (fgets(linha, 100, arquivo) != NULL) {
        char* nome_utente = strtok(linha, ",");
        char* codigo_utente = strtok(NULL, ",");
        char* codigo_medico_utente = strtok(NULL, "\n");
		int cod_utente = atoi(codigo_utente);
		int cod_medico_utente = atoi(codigo_medico_utente);
		
		/*se encontrar começa um loop a ler cada linha do 
		arquivo2 e procurar pelo código do médico que atende ao paciente*/
        if(strcmp(string, nome_utente)==0){
		    while (fgets(linha2, 100, arquivo2) != NULL) {
		        char* nome_medico = strtok(linha2, ",");
		        char* codigo_medico = strtok(NULL, "\n");
				int cod_medico = atoi(codigo_medico);
				//verifica se o código do médico lido corresponde ao código do médico que atende ao paciente
		        if(cod_medico==cod_medico_utente){
		        	printf("\nUtente: %s, Codigo: %d\nMedico: %s, Codigo: %d", nome_utente, codigo_utente, nome_medico, cod_medico);
		        	int cod_utente = Codigo_Utente(nome_utente);
					if(Utente_Esta_Na_Fila_De_Espera(cod_utente) == 1){
						printf("\n\nEste utente esta na fila de espera!");
					}else{
						printf("\n\nEste utente nao esta na fila de espera!");
					}
					break;
				}
		    }
		}
    }
    fclose(arquivo);
	fclose(arquivo2);
    voltar_menu_utentes();
}

int Codigo_Utente(char* Nome_Utente) {
    FILE* arquivo = fopen("utentes.txt", "r");

    char linha[100];
    while (fgets(linha, 100, arquivo) != NULL) {
        char* nome_utente = strtok(linha, ",");
        char* codigo_utente = strtok(NULL, ",");
        char* codigo_medico = strtok(NULL, "\n");
        if (strcmp(nome_utente, Nome_Utente) == 0) {
            int cod_utente = atoi(codigo_utente); //converte a string num numero inteiro
            fclose(arquivo);
            return cod_utente;
        }
    }
    fclose(arquivo);
    return -1;
}

void Remover_Utente(){
	system("CLS");
	char nome[255];
	printf("Nome do Utente de deseja remover: ");
	gets(nome);
	
	int cod_utente = Codigo_Utente(nome);
	//faz com que seja impossivel remover um paciente que esteja numa fila de espera
	if(Utente_Esta_Na_Fila_De_Espera(cod_utente) == 1){	
		printf("\nEste utente esta na fila de espera, remova-o da fila antes de poder remover!");
    	voltar_menu_utentes();
	}
	
	printf("Tem certeza que deseja remover o medico %s? (S/N)\n", nome);
    char resposta = getchar();
    getchar(); // Limpar o buffer de entrada
	
	if (resposta == 'S' || resposta == 's') {
		if(RemoverUtente(nome)){
			printf("Utente removido com sucesso!\n");
		}else{
			printf("Nao foi encontrado esse nome!");		
		}
	} else {
        printf("Operacao cancelada pelo usuario!\n");
    }
    
    voltar_menu_utentes();
}

void Listar_Utentes(){
	system("CLS");
	printf("Lista de Utentes registados: \n");
	char linha[100],linha2[100];
    int i = 0;
    FILE* arquivo = fopen("utentes.txt", "r");
    FILE* arquivo2 = fopen("medicos.txt", "r");
    while (fgets(linha2, 100, arquivo2) != NULL){ //verifica se os ficheiros abriram corretamente
        
		/*As informações do médico incluem seu nome e código, 
		que são armazenados nas variáveis "nome_medico" e "codigo_medico*/
		char* nome_medico = strtok(linha2, ",");
        char* codigo_medico = strtok(NULL, "\n");
        int cod_medico = atoi(codigo_medico);
        printf("\nMedico: %s", nome_medico);
    	while (fgets(linha, 100, arquivo) != NULL) {
	       /*ome do paciente, código do paciente e código do médico que o atende são 
		   armazenados nas variáveis "nome_utente", "codigo_utente" e "codigo_medico_utente"*/
		    char* nome_utente = strtok(linha, ",");
	        char* codigo_utente = strtok(NULL, ",");
	        char* codigo_medico_utente = strtok(NULL, "\n");
			int cod_utente = atoi(codigo_utente);
			int cod_medico_utente = atoi(codigo_medico_utente);
			if(cod_medico == cod_medico_utente){
				printf("\n\tUtente: %s, Codigo: %d", nome_utente, cod_utente);
			}
	    }
	    rewind(arquivo); //para que o loop possa ser executado novamente para o próximo médico
	}
    fclose(arquivo);
    fclose(arquivo2);
    voltar_menu_utentes();
}

char** Verificar_Medico(int* num){
    FILE* arquivo = fopen("utentes.txt", "r");
    FILE* arquivo2 = fopen("medicos.txt", "r");
    
    char** nomes = (char**) malloc(sizeof(char*));
    if (nomes == NULL) {
        printf("Erro ao alocar memoria\n");
        return NULL;
    }
    
    char linha[100],linha2[100];
    int i = 0, encontrou;
    while (fgets(linha, 100, arquivo) != NULL) {
	    encontrou = 0;
        char* nome_utente = strtok(linha, ",");
        char* codigo_utente = strtok(NULL, ",");
        char* codigo_medico_utente = strtok(NULL, "\n");
		int cod_utente = atoi(codigo_utente);
		int cod_medico_utente = atoi(codigo_medico_utente);
		
		rewind(arquivo2); // Volta ao inicio do ficheiro para fazer uma leitura completa
        while (fgets(linha2, 100, arquivo2) != NULL) {
	        char* nome_medico = strtok(linha2, ",");
	        char* codigo_medico = strtok(NULL, "\n");
			int cod_medico = atoi(codigo_medico);
	        if(cod_medico_utente == cod_medico){
	        	encontrou = 1;
	        	break;
			}
	    }
	    
	    
	    if(encontrou == 0){
	    	nomes[i] = (char*) malloc(sizeof(char));
	        strcpy(nomes[i], nome_utente);
	        i++;
		}
    }
    *num = i;
    
    fclose(arquivo);    
    fclose(arquivo2);
    
    return nomes;
}

void Editar_Utente(){
	system("CLS");
	char string[255], string2[255],medico[255];
	int encontrou = 0;
	printf("Nome do Utente que quer editar: ");
	fflush(stdin);
	gets(string);
	
	int cod_utente = Codigo_Utente(string);
	if(Utente_Esta_Na_Fila_De_Espera(cod_utente) == 1){
		printf("\nEste utente esta na fila de espera, remova-o da fila antes de poder editar!");
    	voltar_menu_utentes();
	}
	
    FILE* arquivo = fopen("utentes.txt", "r");
    FILE* arquivoTemp = fopen("temp.txt", "w");
    Utente utente;
    while (fscanf(arquivo, "%[^,],%d,%d\n", utente.nome, &utente.codigo, &utente.codigo_medico) == 3)
    {
    	/*
		Encontramos o utente no ficheiro pelo nome do mesmo e nao copiamos para
		o ficheiro temporario, pois vamos alterar as informações como nome ou medico
		de familia e so depois é que passamos para o ficheiro temporario que ira 
		substituir o ficheiro utentes.txt
		*/
        if (strcmp(utente.nome, string) == 0)
        {
        	encontrou = 1;
			printf("Novo Nome: ");
			fflush(stdin);
			gets(string2);
            int num,i;
			char** nomes_medicos = Nomes_Medicos("medicos.txt",&num);     
		    int opcao = 1;
		    int tecla;
		    while (1) {
		    	system("CLS");
		        printf("Qual é o medico de %s\n\n",string);
			    printf("%s  Manter o mesmo Medico\n", opcao == num+1 ? ">": " ");
		        for (i = 0; i < num; i++) {
			        printf("%s  %s\n", opcao == i+1 ? ">": " ", nomes_medicos[i]);
			    }
		
		        tecla = getch();
		
		        if (tecla == 224) {
		            tecla = getch(); 
		
		            switch (tecla) {
		                case 72: 
		                    opcao = opcao == 1 ? num+1 : opcao - 1;
		                    break;
		                case 80: 
		                    opcao = opcao == num+1 ? 1 : opcao + 1;
		                    break;
		            }
		        } else if (tecla == 13) {
		        	if(opcao!=(num+1)){
		        		strcpy(medico, nomes_medicos[opcao-1]);
		        		int cod_medico = Codigo_Medico(medico);
				    	utente.codigo_medico = cod_medico;
					}
					for (i = 0; i < num; i++) {
				        free(nomes_medicos[i]);
				    }
				    free(nomes_medicos);
				    
				    strcpy(utente.nome, string2);
				    //printf("%s | %d | %d", utente.nome, utente.codigo, utente.codigo_medico);
				    fprintf(arquivoTemp, "%s,%d,%d\n", utente.nome, utente.codigo, utente.codigo_medico);
		            break;
		        }
		    }
        }
        else
        {
            fprintf(arquivoTemp, "%s,%d,%d\n", utente.nome, utente.codigo, utente.codigo_medico);
        }
    }
    fclose(arquivo);
    fclose(arquivoTemp);
    remove("utentes.txt");
    rename("temp.txt", "utentes.txt");
    
    if(encontrou == 0){
    	printf("\nNao foi possivel encontrar esse Utente!");
	}else{
    	printf("\nUtente editado com sucesso!");
	}
    voltar_menu_utentes();
}
