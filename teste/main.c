#include <stdio.h>
#include <locale.h>

#include "estruturas.h"
#include "menus.h"
#include "funcionalidades.h"
#include "base_dados.h"
#include "fila.h"

int main(int argc, char *argv[]){
    setlocale(LC_ALL, "Portuguese");

	Menu_Principal();

    return 0;
}
