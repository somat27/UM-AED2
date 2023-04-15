#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "estruturas.h"
#include "base_dados.h"

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
    	if (strcmp(utente.nome, nomeUtente) != 0)
        {
            fprintf(arquivoTemp, "%s,%d,%d\n", utente.nome, utente.codigo, utente.codigo_medico);
        }else{
        	a = 1;
		}
    }

    fclose(arquivo);
    fclose(arquivoTemp);
    
	remove("utentes.txt");
	rename("temp.txt", "utentes.txt");
	return a;
}

void Editar_Utente(){
	system("CLS");
	char string[255], string2[255],medico[255];
	int encontrou = 0;
	printf("Nome do Utente que quer editar: ");
	fflush(stdin);
	gets(string);
	
    FILE* arquivo = fopen("utentes.txt", "r");
    FILE* arquivoTemp = fopen("temp.txt", "w");
    Utente utente;
    while (fscanf(arquivo, "%[^,],%d,%d\n", utente.nome, &utente.codigo, &utente.codigo_medico) == 3)
    {
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
    getch();
	Menu_Utentes();
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

void Editar_Medico(){
	system("CLS");
	char string[255], string2[255];
	int encontrou = 0;
	printf("Nome do Medico que quer editar: ");
	fflush(stdin);
	gets(string);
	
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
    getch();
	Menu_Medicos();
}

int Gerar_Codigo(){
    srand(time(NULL));

    int num_aleatorio, encontrou = 0;
    char str_num[7]; // 6 dï¿½gitos + \0
    FILE *medico_file, *utente_file;

    do {
        encontrou = 0;
        // Gera um nï¿½mero aleatï¿½rio no intervalo [100000, 999999]
        num_aleatorio = rand() % 999999 + 100000;
        sprintf(str_num, "%d", num_aleatorio); //Para comparar a linha com o codigo visto que lemos a linha como CHAR

        medico_file = fopen("medicos.txt", "r");
        if (medico_file != NULL) {
            char linha[100];
            while (fgets(linha, sizeof(linha), medico_file)) {
                if (strstr(linha, str_num) != NULL) {
                    fclose(medico_file);
                    encontrou = 1;
                    break;
                }
            }
            fclose(medico_file);
        }

        utente_file = fopen("utentes.txt", "r");
        if (utente_file != NULL) {
            char linha[100];
            while (fgets(linha, sizeof(linha), utente_file)) {
                if (strstr(linha, str_num) != NULL) {
                    fclose(utente_file);
                    encontrou = 1;
                    break;
                }
            }
            fclose(utente_file);
        }

        if(encontrou == 0)
        	return num_aleatorio;

    } while (1);
}
