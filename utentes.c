#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "estruturas.h"
#include "utentes.h"

int VerificarNomeUtente(char* nome){	
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
	
	char** nomes_medicos = Nomes_Medicos("medicos.txt",&num); 
	if(num == 0){
		printf("Não há medicos registados!\nRegiste um medico primeiro!");
		printf("\n\nPressione ENTER para continuar...");
    	getchar(); // aguarda a tecla ENTER ser pressionada
		Menu_Utentes();
	} 
	
	printf("Registo de Utente: \n");
	printf("Nome do Utente: ");
	gets(nome);
	if(VerificarNomeUtente(nome) == 1){
		printf("Ja existe um utente chamado \"%s\"",nome);
		printf("\n\nPressione ENTER para continuar...");
    	getchar(); // aguarda a tecla ENTER ser pressionada
		Menu_Utentes();
	}

	Utente utente;
    while (1) {
    	system("CLS");
        printf("Qual é o medico de %s\n\n",nome);
        for (i = 0; i < num; i++) {
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
		    
            break;
        }
    }
	for (i = 0; i < num; i++) {
        free(nomes_medicos[i]);
    }
    free(nomes_medicos);
    printf("\n\nInformações sobre o Utente:");
    printf("\n%s | %d | %d", utente.nome, utente.codigo, utente.codigo_medico);
    printf("\n\nPressione ENTER para continuar...");
    getchar(); // aguarda a tecla ENTER ser pressionada
    
    Menu_Utentes();
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
    while (fgets(linha, 100, arquivo) != NULL) {
        char* nome_utente = strtok(linha, ",");
        char* codigo_utente = strtok(NULL, ",");
        char* codigo_medico_utente = strtok(NULL, "\n");
		int cod_utente = atoi(codigo_utente);
		int cod_medico_utente = atoi(codigo_medico_utente);
        if(strcmp(string, nome_utente)==0){
		    while (fgets(linha2, 100, arquivo2) != NULL) {
		        char* nome_medico = strtok(linha2, ",");
		        char* codigo_medico = strtok(NULL, "\n");
				int cod_medico = atoi(codigo_medico);
		        if(cod_medico==cod_medico_utente){
		        	printf("\nUtente: %s, Codigo: %d\nMedico: %s, Codigo: %d", nome_utente, codigo_utente, nome_medico, cod_medico);
					break;
				}
		    }
		}
    }
    fclose(arquivo);
	fclose(arquivo2);
    printf("\n\nPressione ENTER para continuar...");
    getchar(); // aguarda a tecla ENTER ser pressionada
    Menu_Utentes();
}

void Remover_Utente(){
	system("CLS");
	char nome[255];
	printf("Nome do Utente de deseja remover: ");
	gets(nome);
	
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
    
    printf("\n\nPressione ENTER para continuar...");
    getchar(); // aguarda a tecla ENTER ser pressionada
    Menu_Utentes();
}

void Listar_Utentes(){
	system("CLS");
	printf("Lista de Utentes registados: \n");
	char linha[100];
    int i = 0;
    FILE* arquivo = fopen("utentes.txt", "r");
    while (fgets(linha, 100, arquivo) != NULL) {
        char* nome = strtok(linha, ",");
        char* codigo_str = strtok(NULL, ",");
        char* num_str = strtok(codigo_str, "\n");
		int num_int = atoi(num_str);
        printf("\nNome: %s, Codigo: %d", nome, num_int);
    }
    fclose(arquivo);
    printf("\n\nPressione ENTER para continuar...");
    getchar(); // aguarda a tecla ENTER ser pressionada
    Menu_Utentes();
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
