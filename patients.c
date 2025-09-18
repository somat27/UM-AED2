#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>

#include "structures.h"
#include "menus.h"
#include "doctors.h"
#include "patients.h"
#include "database.h"
#include "queue.h"

static void return_to_patients_menu(){
    printf("\n\nPress ENTER to continue...");
    getchar();
    Patients_Menu();
}

int CheckPatientName(char* name){
    FILE* f = fopen("patients.txt", "r");
    Patient p;
    if(!f) return 0;
    while (fscanf(f, "%[^,],%d,%d\n", p.name, &p.code, &p.doctor_code) == 3)
    {
        if (strcmp(p.name, name) == 0)
        {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int HasDigits(char* str) {
    while (*str) {
        if (isdigit(*str)) {
            return 1;
        }
        str++;
    }
    return 0;
}

void Create_Patient(){
    system("CLS");
    char name[255], doctor_name[255];
    int code = Generate_Code();
    int n, i;
    int option = 1;
    int key;

    char** doctor_names = Doctor_Names("doctors.txt", &n);
    if(n == 0){
        printf("No doctors registered! Register a doctor first!\n");
        return_to_patients_menu();
    }

    printf("Patient registration:\n");
    printf("Patient name: ");
    gets(name);
    if(CheckPatientName(name) == 1){
        printf("A patient named \"%s\" already exists", name);
        return_to_patients_menu();
    }
    if(HasDigits(name)){
        printf("Name cannot contain digits!");
        return_to_patients_menu();
    }

    Patient p;
    while (1) {
        system("CLS");
        printf("Who is %s's doctor?\n\n", name);
        for (i = 0; i < n; i++) {
            printf("%s  %s\n", option == i+1 ? ">": " ", doctor_names[i]);
        }

        key = getch();

        if (key == 224) {
            key = getch(); 

            switch (key) {
                case 72: 
                    option = option == 1 ? n : option - 1;
                    break;
                case 80: 
                    option = option == n ? 1 : option + 1;
                    break;
            }
        } else if (key == 13) {
            strcpy(doctor_name, doctor_names[option-1]);
            int d_code = Doctor_Code(doctor_name);

            strcpy(p.name, name);
            p.code = code;
            p.doctor_code = d_code;
            savePatientToDatabase(&p);

            Ensure_Doctor_Node(d_code, doctor_name);
            Add_Patient_To_Waitlist(p.name, &p);

            break;
        }
    }
    for (i = 0; i < n; i++) {
        free(doctor_names[i]);
    }
    free(doctor_names);
    return_to_patients_menu();
}

void View_Patient(){
    system("CLS");
    char name[255];
    printf("Patient name: ");
    gets(name);
    char line[100], line2[100];
    FILE* fp = fopen("patients.txt", "r");
    FILE* fd = fopen("doctors.txt", "r");
    if(!fp || !fd){ if(fp) fclose(fp); if(fd) fclose(fd); return_to_patients_menu(); return; }

    while (fgets(line, 100, fp) != NULL) {
        char* p_name = strtok(line, ",");
        char* p_code_str = strtok(NULL, ",");
        char* d_code_str = strtok(NULL, "\n");
        int p_code = atoi(p_code_str);
        int d_code = atoi(d_code_str);

        if(strcmp(name, p_name) == 0){
            printf("\nName: %s\nCode: %d", p_name, p_code);
            rewind(fd);
            while (fgets(line2, 100, fd) != NULL) {
                char* d_name = strtok(line2, ",");
                char* d_code_str2 = strtok(NULL, "\n");
                int d_code2 = atoi(d_code_str2);
                if(d_code == d_code2){
                    printf("\nDoctor: %s", d_name);
                    break;
                }
            }
            if(Patient_Is_In_Waitlist(p_code)){
                printf("\nThis patient is in the waitlist!");
            } else {
                printf("\nThis patient is not in the waitlist!");
            }
            break;
        }
    }
    fclose(fp);
    fclose(fd);
    return_to_patients_menu();
}

void Remove_Patient(){
    system("CLS");
    char name[255];
    printf("Name of the patient to remove: ");
    gets(name);

    int p_code = Patient_Code(name);
    if(Patient_Is_In_Waitlist(p_code) == 1){
        printf("\nThis patient is in the waitlist, remove them from the waitlist before removing!");
        return_to_patients_menu();
    }

    printf("Are you sure you want to remove patient %s? (Y/N)\n", name);
    char resp = getchar();
    getchar();
    if (resp == 'Y' || resp == 'y') {
        if(RemovePatientFromDatabase(name)){
            printf("The patient was removed successfully!\n");
        }else{
            printf("Name not found!\n");
        }
    } else {
        printf("Operation canceled!\n");
    }

    return_to_patients_menu();
}

int Patient_Code(char* name){
    FILE* f = fopen("patients.txt", "r");
    if(!f) return -1;
    char line[100];
    while (fgets(line, 100, f) != NULL) {
        char* p_name = strtok(line, ",");
        char* p_code = strtok(NULL, ",");
        if(strcmp(name, p_name) == 0){
            int c = atoi(p_code);
            fclose(f);
            return c;
        }
    }
    fclose(f);
    return -1;
}

void List_Patients(){
    system("CLS");
    printf("Registered patients by doctor:\n");
    char line_p[100], line_d[100];
    FILE* fp = fopen("patients.txt", "r");
    FILE* fd = fopen("doctors.txt", "r");
    if(!fp || !fd){ if(fp) fclose(fp); if(fd) fclose(fd); return_to_patients_menu(); return; }

    while (fgets(line_d, 100, fd) != NULL){
        char* d_name = strtok(line_d, ",");
        char* d_code_str = strtok(NULL, "\n");
        int d_code = atoi(d_code_str);
        printf("\nDoctor: %s", d_name);
        while (fgets(line_p, 100, fp) != NULL) {
            char* p_name = strtok(line_p, ",");
            char* p_code_str = strtok(NULL, ",");
            char* p_d_code_str = strtok(NULL, "\n");
            int p_code = atoi(p_code_str);
            int p_d_code = atoi(p_d_code_str);
            if(d_code == p_d_code){
                printf("\n\tPatient: %s, Code: %d", p_name, p_code);
            }
        }
        rewind(fp);
    }
    fclose(fp);
    fclose(fd);
    return_to_patients_menu();
}

char** Patients_With_Invalid_Doctor(int* count){
    FILE* fp = fopen("patients.txt", "r");
    FILE* fd = fopen("doctors.txt", "r");
    
    char** names = (char**) malloc(sizeof(char*));
    if (names == NULL) {
        printf("Memory allocation error\n");
        *count = 0;
        if(fp) fclose(fp);
        if(fd) fclose(fd);
        return NULL;
    }
    
    char line[100], line2[100];
    int i = 0, found;
    if(!fp || !fd){ *count = 0; if(fp) fclose(fp); if(fd) fclose(fd); return names; }
    while (fgets(line, 100, fp) != NULL) {
        found = 0;
        char* p_name = strtok(line, ",");
        char* p_code = strtok(NULL, ",");
        char* p_d_code = strtok(NULL, "\n");
        int p_code_i = atoi(p_code);
        int p_d_code_i = atoi(p_d_code);
        
        rewind(fd);
        while (fgets(line2, 100, fd) != NULL) {
            char* d_name = strtok(line2, ",");
            char* d_code = strtok(NULL, "\n");
            int d_code_i = atoi(d_code);
            if(p_d_code_i == d_code_i){
                found = 1;
                break;
            }
        }
        if(found == 0){
            names = realloc(names, (i+1) * sizeof(char*));
            names[i] = (char*) malloc(strlen(p_name)+1);
            strcpy(names[i], p_name);
            i++;
        }
    }
    *count = i;
    fclose(fp);
    fclose(fd);
    return names;
}

void Edit_Patient(){
    system("CLS");
    char name[255], new_name[255], doctor_name[255];
    int found = 0;
    printf("Patient to edit: ");
    fflush(stdin);
    gets(name);

    int p_code = Patient_Code(name);
    if(Patient_Is_In_Waitlist(p_code) == 1){
        printf("\nThis patient is in the waitlist, remove them before editing!\n");
        return_to_patients_menu();
    }

    FILE* fp = fopen("patients.txt", "r");
    FILE* tmp = fopen("temp.txt", "w");
    Patient p;
    while (fscanf(fp, "%[^,],%d,%d\n", p.name, &p.code, &p.doctor_code) == 3)
    {
        if (strcmp(p.name, name) == 0)
        {
            found = 1;
            printf("New name: ");
            fflush(stdin);
            gets(new_name);
            if(HasDigits(new_name)){
                printf("Name cannot contain digits!");
                return_to_patients_menu();
            }
            int n, i;
            char** doctor_names = Doctor_Names("doctors.txt", &n);
            int option = 1;
            int key;
            while (1) {
                system("CLS");
                printf("Who is %s's doctor?\n\n", name);
                printf("%s  Keep the same doctor\n", option == n+1 ? ">": " ");
                for (i = 0; i < n; i++) {
                    printf("%s  %s\n", option == i+1 ? ">": " ", doctor_names[i]);
                }

                key = getch();

                if (key == 224) {
                    key = getch(); 

                    switch (key) {
                        case 72: 
                            option = option == 1 ? n+1 : option - 1;
                            break;
                        case 80: 
                            option = option == n+1 ? 1 : option + 1;
                            break;
                    }
                } else if (key == 13) {
                    if(option != (n+1)){
                        strcpy(doctor_name, doctor_names[option-1]);
                        int d_code = Doctor_Code(doctor_name);
                        p.doctor_code = d_code;
                    }
                    for (i = 0; i < n; i++) {
                        free(doctor_names[i]);
                    }
                    free(doctor_names);

                    strcpy(p.name, new_name);
                    fprintf(tmp, "%s,%d,%d\n", p.name, p.code, p.doctor_code);
                    break;
                }
            }
        }
        else
        {
            fprintf(tmp, "%s,%d,%d\n", p.name, p.code, p.doctor_code);
        }
    }
    fclose(fp);
    fclose(tmp);
    remove("patients.txt");
    rename("temp.txt", "patients.txt");
    
    if(found == 0){
        printf("\nPatient not found!");
    }else{
        printf("\nPatient edited successfully!");
    }
    return_to_patients_menu();
}

