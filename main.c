#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <conio.h>

#include "structures.h"
#include "menus.h"
#include "doctors.h"
#include "patients.h"
#include "database.h"
#include "queue.h"

void createDataFiles(){
    FILE* f1 = fopen("doctors.txt", "a");
    FILE* f2 = fopen("patients.txt", "a");
    if(f1) fclose(f1);
    if(f2) fclose(f2);
}

int main(int argc, char *argv[]){
    setlocale(LC_ALL, "");

    createDataFiles();
    Main_Menu();

    return 0;
}

