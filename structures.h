typedef struct patient {
    char name[255];
    int code;
    int doctor_code;
    struct patient* next;     // Pointer to next patient 
} Patient;

typedef struct doctor {
    char name[255];
    int code;
    Patient* waitlist;		// Pointer to the first patient in the waitlist
    struct doctor* next;		// Pointer to the next doctor
} Doctor;
