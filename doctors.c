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

static void return_to_doctors_menu(){
    printf("\n\nPress ENTER to continue...");
    getchar();
    Doctors_Menu();
}

int CheckDoctorName(char* name){
    FILE* f = fopen("doctors.txt", "r");
    Doctor d;
    if(!f) return 0;
    while (fscanf(f, "%[^,],%d\n", d.name, &d.code) == 2)
    {
        if (strcmp(d.name, name) == 0)
        {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

char** Doctor_Names(char* filename, int* n_doctors) {
    FILE* f = fopen("doctors.txt", "r");
    char** names = NULL;
    char line[100];
    int i = 0;
    if(!f){
        *n_doctors = 0;
        return NULL;
    }
    while (fgets(line, 100, f) != NULL) {
        char* name = strtok(line, ",");
        names = realloc(names, (i + 1) * sizeof(char*));
        names[i] = strdup(name);
        i++;
    }
    *n_doctors = i;
    fclose(f);
    return names;
}

int Doctor_Code(char* doctor_name) {
    FILE* f = fopen("doctors.txt", "r");
    char line[100];
    if(!f) return -1;
    while (fgets(line, 100, f) != NULL) {
        char* name = strtok(line, ",");
        char* code = strtok(NULL, "\n");
        if (strcmp(name, doctor_name) == 0) {
            int d_code = atoi(code);
            fclose(f);
            return d_code;
        }
    }
    fclose(f);
    return -1;
}

void Create_Doctor(){
    system("CLS");
    char name[255];
    printf("Doctor registration:\n");
    printf("Doctor name: ");
    fgets(name, 255, stdin);
    name[strcspn(name, "\n")] = '\0';
    if(CheckDoctorName(name) == 1){
        printf("A doctor named \"%s\" already exists", name);
        return_to_doctors_menu();
    }
    if(HasDigits(name)){
        printf("Name cannot contain digits!");
        return_to_doctors_menu();
    }
    int code = Generate_Code();
    Doctor d;
    strcpy(d.name, name);
    d.code = code;
    saveDoctorToDatabase(&d);
    printf("Doctor created successfully!");
    return_to_doctors_menu();
}

void View_Doctor(){
    system("CLS");
    char doctor_name[255];
    printf("Doctor name: ");
    fgets(doctor_name, 255, stdin);
    doctor_name[strcspn(doctor_name, "\n")] = '\0';

    FILE* fd = fopen("doctors.txt", "r");
    FILE* fp = fopen("patients.txt", "r");
    if(!fd || !fp){
        if(fd) fclose(fd);
        if(fp) fclose(fp);
        return_to_doctors_menu();
        return;
    }

    char doctor_line[255];
    while (fgets(doctor_line, 255, fd) != NULL) {
        char* name = strtok(doctor_line, ",");
        char* code_str = strtok(NULL, "\n");
        int d_code = atoi(code_str);

        if(strcmp(doctor_name, name) == 0) {
            printf("\n\nDoctor: %s, Code: %d\nPatient list:", name, d_code);

            char patient_line[255];
            while (fgets(patient_line, 255, fp) != NULL) {
                char* p_name = strtok(patient_line, ",");
                char* p_code_str = strtok(NULL, ",");
                char* p_doctor_code_str = strtok(NULL, "\n");
                int p_code = atoi(p_code_str);
                int p_d_code = atoi(p_doctor_code_str);

                if(d_code == p_d_code) {
                    printf("\n Name: %s, Code: %d", p_name, p_code);
                    int code = Patient_Code(p_name);
                    if(Patient_Is_In_Waitlist(code) == 1){
                        printf(" - This patient is in the waitlist!");
                    }else{
                        printf(" - This patient is not in the waitlist!");
                    }
                }
            }
            fseek(fp, 0, SEEK_SET);
        }
    }
    fclose(fd);
    fclose(fp);

    return_to_doctors_menu();
}

void Remove_Doctor(){
    system("CLS");
    char name[255];
    printf("Name of the doctor to remove: ");
    gets(name);

    int d_code = Doctor_Code(name);
    if(Check_Doctor_Waitlist(d_code) == 1){
        printf("\nThis doctor has patients in the waitlist. Remove them before removing the doctor!");
        return_to_doctors_menu();
    }

    printf("Are you sure you want to remove doctor %s? (Y/N)\n", name);
    char resp = getchar();
    getchar();

    if (resp == 'Y' || resp == 'y') {
        if(RemoveDoctorFromDatabase(name)){
            printf("Doctor removed successfully!\n");
        }else{
            printf("Name not found!\n");
        }
    } else {
        printf("Operation canceled!\n");
    }

    return_to_doctors_menu();
}

void List_Doctors(){
    system("CLS");
    printf("Registered doctors:\n");
    char line[100];
    FILE* f = fopen("doctors.txt", "r");
    if(!f){
        return_to_doctors_menu();
        return;
    }
    while (fgets(line, 100, f) != NULL) {
        char* name = strtok(line, ",");
        char* code_str = strtok(NULL, "\n");
        int code = atoi(code_str);
        printf("\nName: %s, Code: %d", name, code);
    }
    fclose(f);
    return_to_doctors_menu();
}

void Edit_Doctor(){
    system("CLS");
    char name[255], new_name[255];
    int found = 0;
    printf("Doctor to edit: ");
    fflush(stdin);
    gets(name);

    int d_code = Doctor_Code(name);
    if(Check_Doctor_Waitlist(d_code) == 1){
        printf("\nThis doctor has patients in the waitlist. Remove them before editing this doctor!");
        return_to_doctors_menu();
    }

    FILE* f = fopen("doctors.txt", "r");
    FILE* tmp = fopen("temp.txt", "w");
    Doctor d;
    while (fscanf(f, "%[^,],%d\n", d.name, &d.code) == 2)
    {
        if (strcmp(d.name, name) == 0)
        {
            found = 1;
            printf("New name: ");
            fflush(stdin);
            gets(new_name);
            if(HasDigits(new_name)){
                printf("Name cannot contain digits!");
                return_to_doctors_menu();
            }
            strcpy(d.name, new_name);
            fprintf(tmp, "%s,%d\n", d.name, d.code);
        }
        else
        {
            fprintf(tmp, "%s,%d\n", d.name, d.code);
        }
    }
    fclose(f);
    fclose(tmp);
    remove("doctors.txt");
    rename("temp.txt", "doctors.txt");

    if(found == 0){
        printf("\nDoctor not found!");
    }else{
        printf("\nDoctor edited successfully!");
    }
    return_to_doctors_menu();
}
