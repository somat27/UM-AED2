// Requires structures.h
#include "structures.h"

void savePatientToDatabase(struct patient *patient);
int RemovePatientFromDatabase(char* patient_name);
void saveDoctorToDatabase(struct doctor* doctor);
int RemoveDoctorFromDatabase(char* doctor_name);
int Generate_Code();
void Change_Patients_Doctor(char** patient_names, char* doctor_name, int patients_count);
