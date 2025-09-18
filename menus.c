#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <conio.h>
#include <string.h>

#include "structures.h"
#include "menus.h"
#include "doctors.h"
#include "patients.h"
#include "database.h"
#include "queue.h"

void Main_Menu(){    
    system("cls");
    Doctor_Change_Menu();
    
    int option = 1;
    int key;

    while (1) {
        system("cls");
        ASCII_Print(1); 
        printf("\n\n");
        printf("\t%s  Patient Attendance\n", option == 1 ? ">": " ");
        printf("\t%s  Patients Menu\n", option == 2 ? ">": " ");
        printf("\t%s  Doctors Menu\n", option == 3 ? ">": " ");
        printf("\n\t%s  Exit\n", option == 4 ? ">": " ");

        key = getch();

        if (key == 224) {
            key = getch();

            switch (key) {
                case 72:
                    option = option == 1 ? 4 : option - 1; 
                    break;
                case 80:
                    option = option == 4 ? 1 : option + 1;
                    break;
            }
        } else if (key == 13) {
            switch (option) {
                case 1:
                    Attendance_Menu(); 
                    break;
                case 2: 
                    Patients_Menu(); 
                    break;
                case 3: 
                    Doctors_Menu(); 
                    break;
                case 4: 
                    Exit_Program(); 
                    break;
            }
            break;
        }
    }
}

void Exit_Program(){   
    exit(0);
}

void ASCII_Print(int a){
    fflush(stdin);
    printf("\n");
    if(a==1){
        printf("\n\t\t\t  SGAU");
        printf("\n\t  Health Center — Patients and Doctors Management");
    }else if(a==2){
        printf("\n\t  Patients Menu");
    }else if(a==3){
        printf("\n\t  Doctors Menu");
    }
    printf("\n");
}

void Patients_Menu(){
    int option=1, key;

    while (1) {
        system("cls");
        ASCII_Print(2);
        printf("\n\n");
        printf("\t%s  Create Patient\n", option == 1 ? ">": " ");
        printf("\t%s  Edit Patient \n", option == 2 ? ">": " ");
        printf("\t%s  View Patient\n", option == 3 ? ">": " ");
        printf("\t%s  Remove Patient\n", option == 4 ? ">": " ");
        printf("\t%s  List Patients\n", option == 5 ? ">": " ");
        printf("\n\t%s  Main Menu\n", option == 6 ? ">": " ");

        key = getch();
        if (key == 224) { 
            key = getch();
            switch (key) {
                case 72:
                    option = option == 1 ? 6 : option - 1; 
                    break;
                case 80: 
                    option = option == 6 ? 1 : option + 1;
                    break;
            }
        } else if (key == 13) {
            switch(option){
                case 1:
                    Create_Patient();
                    break;
                case 2:
                    Edit_Patient();
                    break;
                case 3:
                    View_Patient();
                    break;
                case 4:
                    Remove_Patient();
                    break;
                case 5:
                    List_Patients();
                    break;
                case 6:
                    Main_Menu();
                    break;
            }
            break;
        }
    }
}

void Doctors_Menu(){
    int option = 1;
    int key;

    while (1) {
        system("cls");
        ASCII_Print(3);
        printf("\n\n");
        printf("\t%s  Create Doctor\n", option == 1 ? ">": " ");
        printf("\t%s  Edit Doctor \n", option == 2 ? ">": " ");
        printf("\t%s  View Doctor\n", option == 3 ? ">": " ");
        printf("\t%s  Remove Doctor\n", option == 4 ? ">": " ");
        printf("\t%s  List Doctors\n", option == 5 ? ">": " ");
        printf("\n\t%s  Main Menu\n", option == 6 ? ">": " ");

        key = getch();
        if (key == 224) { 
            key = getch();
            switch (key) {
                case 72:
                    option = option == 1 ? 6 : option - 1; 
                    break;
                case 80: 
                    option = option == 6 ? 1 : option + 1;
                    break;
            }
        } else if (key == 13) {
            switch(option){
                case 1:
                    Create_Doctor();
                    break;
                case 2:
                    Edit_Doctor();
                    break;
                case 3:
                    View_Doctor();
                    break;
                case 4:
                    Remove_Doctor();
                    break;
                case 5:
                    List_Doctors();
                    break;
                case 6:
                    Main_Menu();
                    break;
            }
            break;
        }
    }
}

void Doctor_Change_Menu(){
    int count, count_doctors, i, option=1, key;
    char** names_invalid = Patients_With_Invalid_Doctor(&count);
    if(count != 0){
        printf("\nPatients with invalid doctor: ");
        for(i=0;i<count;i++){
            printf("\n%s", names_invalid[i]);
        }
        printf("\nChoose a new doctor for these patients!");
        printf("\n\nPress ENTER to continue...");
        getchar();
        char** doctor_names = Doctor_Names("doctors.txt", &count_doctors);
        while (1) {
            system("CLS");
            printf("Available doctors: \n");
            for (i = 0; i < count_doctors; i++) {
                printf("%s  %s\n", option == i+1 ? ">": " ", doctor_names[i]);
            }

            key = getch();

            if (key == 224) {
                key = getch(); 

                switch (key) {
                    case 72: 
                        option = option == 1 ? count_doctors : option - 1;
                        break;
                    case 80: 
                        option = option == count_doctors ? 1 : option + 1;
                        break;
                }
            } else if (key == 13) {
                Change_Patients_Doctor(names_invalid, doctor_names[option-1], count);
                break;
            }
        }
        for (i = 0; i < count_doctors; i++) {
            free(doctor_names[i]);
        }
        free(doctor_names);
    }
    for (i = 0; i < count; i++) {
        free(names_invalid[i]);
    }
    free(names_invalid);
}

