// Requires structures.h
#include "structures.h"

void Waitlist_Add_Menu();
void Ensure_Doctor_Node(int code, char* name);
void Add_Patient_To_Waitlist(char* name, struct patient* patient);
void Waitlist_List_By_Doctor_Menu();
void Remove_First_Patient_From_Waitlist(struct doctor* doctor);
void Waitlist_Remove_From_Doctor_Menu();
int Patient_Is_In_Waitlist(int patient_code);
int Check_Doctor_Waitlist(int doctor_code);
