#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <conio.h>

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

int main(int argc, char *argv[]){
    setlocale(LC_ALL, "Portuguese");  //Configuração regional para Português 

	criarFiles();
	Menu_Principal();

    return 0;
}
