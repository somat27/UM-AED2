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

Doctor *doctors = NULL;

static void return_to_queue_menu(){
    printf("\n\nPress ENTER to continue...");
    getchar();
    Attendance_Menu();
}

Doctor* Find_Doctor_With_Largest_Waitlist(int *count) {
    Doctor* curr = doctors;
    Doctor* best = NULL;
    int maxCount = -1;
    if(curr == NULL){
        *count = 0;
        return NULL;
    }
    while (curr != NULL) {
        Patient* p = curr->waitlist;
        int n = 0;
        while (p != NULL) {
            n++;
            p = p->next;
        }
        if (n > maxCount) {
            maxCount = n;
            best = curr;
        }
        curr = curr->next;
    }
    *count = maxCount;
    return best;
}

int Patient_Is_In_Waitlist(int patient_code) {
    Doctor* d = doctors;
    while (d != NULL) {
        Patient* p = d->waitlist;
        while (p != NULL) {
            if (p->code == patient_code) {
                return 1;
            }
            p = p->next;
        }
        d = d->next;
    }
    return 0;
}

int Check_Doctor_Waitlist(int doctor_code) {
    Doctor* d = doctors;
    while (d != NULL) {
        if (d->code == doctor_code) {
            Patient* p = d->waitlist;
            if (p == NULL) {
                return 0;
            } else {
                return 1;
            }
        }
        d = d->next;
    }
    return -1;
}

void Attendance_Menu(){
    int option = 1;
    int key;

    while (1) {
        system("cls");
        ASCII_Print(2);
        printf("\n\n");
        int qty;
        Doctor* busiest = Find_Doctor_With_Largest_Waitlist(&qty);
        if(busiest != NULL && qty != 0){
            printf("\t%s has %d patients waiting (largest queue).\n\n", busiest->name, qty);
        }
        printf("\t%s  Add patient to waitlist\n", option == 1 ? ">": " ");
        printf("\t%s  Remove next from waitlist\n", option == 2 ? ">": " ");
        printf("\t%s  View waitlist by doctor\n", option == 3 ? ">": " ");
        printf("\n\t%s  Main Menu\n", option == 4 ? ">": " ");

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
            switch(option){
                case 1:
                    Waitlist_Add_Menu();
                    break;
                case 2:
                    Waitlist_Remove_From_Doctor_Menu();
                    break;
                case 3:
                    Waitlist_List_By_Doctor_Menu();
                    break;
                case 4:
                    Main_Menu();
                    break;
            }
            break;
        }
    }
}

void Ensure_Doctor_Node(int code, char *name){
    Doctor* curr = doctors;
    while (curr != NULL) {
        if (curr->code == code) {
            return;
        }
        curr = curr->next;
    }
    Doctor* nd = malloc(sizeof(Doctor));
    nd->code = code;
    strcpy(nd->name, name);
    nd->waitlist = NULL;
    nd->next = doctors;
    doctors = nd;
}

void Add_Patient_To_Waitlist(char *name, Patient *patient){
    Patient* node = malloc(sizeof(Patient));
    node->code = patient->code;
    node->doctor_code = patient->doctor_code;
    strcpy(node->name, patient->name);
    node->next = NULL;

    Doctor* d = doctors;
    while (d != NULL) {
        if (d->code == patient->doctor_code) {
            Patient* p = d->waitlist;
            if (p == NULL) { 
                d->waitlist = node;
                printf("\n%s was added to %s's waitlist\n", name, d->name);
            } else {
                while (p != NULL) {
                    if (p->code == node->code) {
                        printf("\nThis patient is already in the waitlist!");
                        break;
                    }
                    if (p->next == NULL) {
                        p->next = node;
                        printf("\n%s was added to %s's waitlist\n", name, d->name);
                        break;
                    }
                    p = p->next;
                }
            }
            break;
        }
        d = d->next;
    }
}

void Waitlist_Add_Menu(){
    system("CLS");
    ASCII_Print(2);
    char name[255];
    Patient p;
    printf("Patient name to add to waitlist: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';
    if (CheckPatientName(name) == 1) {
        char line[100];
        int code = 0;
        FILE* fp = fopen("patients.txt", "r");
        FILE* fd = fopen("doctors.txt", "r");
        while (fscanf(fp, "%[^,],%d,%d\n", p.name, &p.code, &p.doctor_code) == 3) {
            if (strcmp(name, p.name) == 0) {
                while (fscanf(fd, "%[^,],%d\n", line, &code) == 2) {
                    if (code == p.doctor_code) {
                        Ensure_Doctor_Node(code, line);
                        Add_Patient_To_Waitlist(name, &p);
                        break;
                    }
                }
            }
        }
        fclose(fp);
        fclose(fd);
        return_to_queue_menu();
    } else {
        printf("Patient not found!");
        return_to_queue_menu();
    }
}

void Waitlist_List_By_Doctor_Menu() {
    system("CLS");
    int n, i, option = 0, key;
    char** names = Doctor_Names("doctors.txt", &n);

    if (n == 0) {
        printf("No doctors registered! Register a doctor first.");
        printf("\n\nPress ENTER to continue...");
        getchar();
        Attendance_Menu();
        return;
    }

    while (1) {
        system("CLS");
        printf("Doctors list:\n\n");
        for (i = 0; i < n; i++) {
            printf("%s %s\n", option == i ? ">" : " ", names[i]);
        }

        key = getch();

        if (key == 224) {
            key = getch(); 

            switch (key) {
                case 72:
                    option = (option + n - 1) % n;
                    break;
                case 80:
                    option = (option + 1) % n;
                    break;
            }
        } else if (key == 13) {
            system("CLS");
            int doctor_code = Doctor_Code(names[option]);
            int qty = 0;
            Doctor* d = doctors;
            while (d != NULL) {
                if(d->code == doctor_code){
                    printf("Waitlist for doctor: %s\n\n", d->name);
                    Patient* p = d->waitlist;
                    while (p != NULL) {
                        printf("\nName: %s, Code: %d", p->name, p->code);
                        p = p->next;
                        qty++;
                    }
                }
                d = d->next;
            }
            if(qty == 0){
                printf("\nNo one is in the waitlist!");
            } else {
                printf("\n\nTotal of %d patients in the waitlist", qty);
            }
            break;
        }
    }

    for (i = 0; i < n; i++) {
        free(names[i]);
    }
    free(names);
    return_to_queue_menu();
}

void Remove_First_Patient_From_Waitlist(Doctor *doctor) {
    Patient *first = doctor->waitlist;
    if (first == NULL) {
        printf("Waitlist is empty.\n");
        return;
    }
    doctor->waitlist = first->next;
    free(first);
    printf("First patient from %s's waitlist removed.\n", doctor->name);
}

void Waitlist_Remove_From_Doctor_Menu() {
    system("CLS");
    int n, i, option = 0, key;
    char** names = Doctor_Names("doctors.txt", &n);

    if (n == 0) {
        printf("No doctors registered! Register a doctor first.");
        printf("\n\nPress ENTER to continue...");
        getchar();
        Attendance_Menu();
        return;
    }

    while (1) {
        system("CLS");
        printf("Doctors list:\n\n");
        for (i = 0; i < n; i++) {
            printf("%s %s\n", option == i ? ">" : " ", names[i]);
        }

        key = getch();

        if (key == 224) {
            key = getch(); 

            switch (key) {
                case 72:
                    option = (option + n - 1) % n;
                    break;
                case 80:
                    option = (option + 1) % n;
                    break;
            }
        } else if (key == 13) {
            int doctor_code = Doctor_Code(names[option]);
            Doctor* d = doctors;
            while (d != NULL) {
                if (d->code == doctor_code) {
                    if (d->waitlist == NULL) {
                        printf("Error: No one in the waitlist!\n");
                    }else{
                        Remove_First_Patient_From_Waitlist(d);
                    }
                    break;
                }
                d = d->next;
            }
            break;
        }
    }

    for (i = 0; i < n; i++) {
        free(names[i]);
    }
    free(names);
    return_to_queue_menu();
}

