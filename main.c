#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

#include "estruturas.h"
#include "menus.h"
#include "medicos.h"
#include "utentes.h"
#include "base_dados.h"
#include "fila.h"

void criarFiles(){
	FILE* arquivo = fopen("medicos.txt", "a");
	FILE* arquivo2 = fopen("utentes.txt", "a");
	fclose(arquivo);
	fclose(arquivo2);
}

void Sair_Programa(){   
	exit(0);
}

int main(int argc, char *argv[]){
    setlocale(LC_ALL, "Portuguese");

	criarFiles();
	Menu_Principal();

    return 0;
}
