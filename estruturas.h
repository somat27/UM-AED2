typedef struct utente {
    char nome[255];
    int codigo;
    int codigo_medico;
    struct utente* proximo;
} Utente;

typedef struct medico {
    char nome[255];
    int codigo;
    Utente* fila_espera;
    struct medico* proximo;
} Medico;
