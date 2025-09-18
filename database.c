#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <conio.h>
#include <string.h>

#include "structures.h"
#include "database.h"

void savePatientToDatabase(Patient* patient){
    FILE* f = fopen("patients.txt", "a");
    fprintf(f, "%s,%d,%d\n", patient->name, patient->code, patient->doctor_code);
    fclose(f);
}

int RemovePatientFromDatabase(char* patient_name){
    int removed = 0;
    FILE* f = fopen("patients.txt", "r");
    FILE* tmp = fopen("temp.txt", "w");

    Patient p;
    while (fscanf(f, "%[^,],%d,%d\n", p.name, &p.code, &p.doctor_code) == 3)
    {
        if (strcmp(p.name, patient_name) != 0){
            fprintf(tmp, "%s,%d,%d\n", p.name, p.code, p.doctor_code);
        } else {
            removed = 1;
        }
    }

    fclose(f);
    fclose(tmp);

    remove("patients.txt");
    rename("temp.txt", "patients.txt");
    return removed;
}

void saveDoctorToDatabase(Doctor* doctor){
    FILE* f = fopen("doctors.txt", "a");
    fprintf(f, "%s,%d\n", doctor->name, doctor->code);
    fclose(f);
}

int RemoveDoctorFromDatabase(char* doctor_name){
    int removed = 0;
    FILE* f = fopen("doctors.txt", "r");
    FILE* tmp = fopen("temp.txt", "w");

    Doctor d;
    while (fscanf(f, "%[^,],%d\n", d.name, &d.code) == 2)
    {
        if (strcmp(d.name, doctor_name) != 0)
        {
            fprintf(tmp, "%s,%d\n", d.name, d.code);
        } else {
            removed = 1;
        }
    }

    fclose(f);
    fclose(tmp);

    remove("doctors.txt");
    rename("temp.txt", "doctors.txt");
    return removed;
}

int Generate_Code(){
    srand((unsigned int)time(NULL));

    int rnd, exists = 0;
    char line[100];

    FILE* f = fopen("patients.txt", "r");
    if(!f){
        // If file doesn't exist yet, just generate a number
        return (rand() % 999999) + 100000;
    }

    do {
        exists = 0;
        rnd = rand() % 999999 + 100000; 

        while (fgets(line, sizeof(line), f)) {
            char* patient_name = strtok(line, ",");
            char* patient_code = strtok(NULL, ",");
            char* doctor_code = strtok(NULL, "\n");
            int p_code = atoi(patient_code);
            int d_code = atoi(doctor_code);
            if (p_code == rnd || d_code == rnd) {
                exists = 1;
                break;
            }
        }

        if(exists == 0){
            fclose(f);
            return rnd;
        }
        fseek(f, 0, SEEK_SET);

    } while (1);
}

void Change_Patients_Doctor(char** patient_names, char* doctor_name, int patients_count){
    int i, match;

    FILE* fd = fopen("doctors.txt", "r");
    FILE* fp = fopen("patients.txt", "r");
    FILE* tmp = fopen("temp.txt", "w");

    char line[100], line2[100];
    int doctor_code;
    Patient p;

    while (fgets(line, 100, fd) != NULL) {
        char* name = strtok(line, ",");
        char* code = strtok(NULL, "\n");
        if(strcmp(name, doctor_name) == 0){
            doctor_code = atoi(code);
            break;
        }
    }
    fclose(fd);

    while (fgets(line2, 100, fp) != NULL) {
        match = 0;
        char* patient_name = strtok(line2, ",");
        char* patient_code = strtok(NULL, ",");
        char* patient_doctor_code = strtok(NULL, "\n");
        int p_code = atoi(patient_code);
        int p_d_code = atoi(patient_doctor_code);
        strcpy(p.name, patient_name);
        p.code = p_code;
        for(i=0;i<patients_count;i++){
            if(strcmp(patient_names[i], patient_name) == 0){
                match = 1;
            }
        }
        if(match){
            p.doctor_code = doctor_code;
            fprintf(tmp, "%s,%d,%d\n", p.name, p.code, p.doctor_code);
        }else{
            p.doctor_code = p_d_code;
            fprintf(tmp, "%s,%d,%d\n", p.name, p.code, p.doctor_code);
        }

    }
    fclose(fp);
    fclose(tmp);
    remove("patients.txt");
    rename("temp.txt", "patients.txt");
}

