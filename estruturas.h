typedef struct utente {
    char nome[255];
    int codigo;
    int codigo_medico;
    struct utente* proximo;     //Apontador para o proximo utente 
} Utente;

typedef struct medico {
    char nome[255];
    int codigo;
    Utente* fila_espera;		//Apontador para o primeiro utente da fila de espera
    struct medico* proximo;		//Apontador para o proximo medico
} Medico;
