#include "main.h"

int get_file_noise(FILE *fp) {
    unsigned char b = 0;

    if (fp == NULL) {
        /* Caso di emergenza: se per qualche motivo fp è NULL,
           usciamo con errore (meglio fallire che generare password sbagliate) */
        fprintf(stderr, "Errore: sorgente di casualità non disponibile.\n");
        exit(EXIT_FAILURE);
    }

    if (fread(&b, 1, 1, fp) != 1) {
        fprintf(stderr, "Errore in lettura da /dev/urandom.\n");
        exit(EXIT_FAILURE);
    }

    return (int)b;
}

/* 7 lettere (maiuscole/minuscole) */
void d20_d12_d6(unsigned char *ascii, FILE *fp) {
    for (int i = 0; i < 7; i++) {
        int r = get_file_noise(fp);
        int upper = r & 1;  /* bit meno significativo: 0 = minuscolo, 1 = maiuscolo */
        int letter = get_file_noise(fp) % 26;

        if (upper) {
            ascii[i] = 'A' + letter;
        } else {
            ascii[i] = 'a' + letter;
        }
    }
    ascii[7] = '\0';
}

/* 3 cifre numeriche */
void d10(char *numbers, FILE *fp) {
    for (int i = 0; i < 3; i++) {
        int r = get_file_noise(fp);
        numbers[i] = '0' + (r % 10);
    }
    numbers[3] = '\0';
}

/* 2 caratteri speciali presi da una lista fissa */
void d8(char *special, FILE *fp) {
    const char specials[] = "!@#$%^&*";  /* 8 caratteri speciali */
    int len = (int)(sizeof(specials) - 1);  /* escludo il terminatore '\0' */

    for (int i = 0; i < 2; i++) {
        int r = get_file_noise(fp);
        special[i] = specials[r % len];
    }
    special[2] = '\0';
}

/* Genera una permutazione casuale dei numeri 1..4 */
void d4(int *order, FILE *fp) {
    int used[5] = {0};  /* indici da 1 a 4 usati; 0 = libero, 1 = usato */
    int index = 0;

    while (index < 4) {
        int r = get_file_noise(fp);
        int val = (r % 4) + 1;  /* 1..4 */

        if (!used[val]) {
            used[val] = 1;
            order[index] = val;
            index++;
        }
    }
}

int main(void) {
    /* 7 lettere + 3 cifre + 2 speciali + 1 '@' = 13 + '\0' = 14 */
    char password[14];
    int order[4] = {0, 0, 0, 0};
    int index = 0;
    unsigned char ascii[8];
    char numbers[4];
    char special[3];

    /* Apro /dev/urandom in sola lettura binaria */
    FILE *fp = fopen("/dev/urandom", "rb");
    if (fp == NULL) {
        fprintf(stderr, "Errore: impossibile aprire /dev/urandom.\n");
        return EXIT_FAILURE;
    }

    /* Genero i vari blocchi */
    d4(order, fp);
    d8(special, fp);
    d10(numbers, fp);
    d20_d12_d6(ascii, fp);

    /* Ricompongo la password secondo l’ordine scelto:
       1 = blocco lettere (7)
       2 = blocco numeri (3)
       3 = blocco speciali (2)
       4 = carattere '@' singolo
    */
    for (int i = 0; i < 4; i++) {
        switch (order[i]) {
            case 1:  /* lettere */
                for (int j = 0; j < 7; j++) {
                    password[index + j] = (char)ascii[j];
                }
                index += 7;
                break;

            case 2:  /* numeri */
                for (int j = 0; j < 3; j++) {
                    password[index + j] = numbers[j];
                }
                index += 3;
                break;

            case 3:  /* speciali */
                for (int j = 0; j < 2; j++) {
                    password[index + j] = special[j];
                }
                index += 2;
                break;

            case 4:  /* singolo '@' */
                password[index] = '@';
                index++;
                break;
        }
    }

    /* Terminatore di stringa */
    password[index] = '\0';

    printf("Generated password: %s\n", password);

    fclose(fp);
    return EXIT_SUCCESS;
}
