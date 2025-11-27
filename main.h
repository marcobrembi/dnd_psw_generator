#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>

/* Legge 1 byte sicuro da /dev/urandom (tramite FILE* aperto in main) */
int get_file_noise(FILE *fp);

/* Genera un ordine casuale dei 4 blocchi (1..4) */
void d4(int *order, FILE *fp);

/* Genera 2 caratteri speciali + terminatore */
void d8(char *speciali, FILE *fp);

/* Genera 3 cifre numeriche + terminatore */
void d10(char *numbers, FILE *fp);

/* Genera 7 lettere alfabetiche (maiuscole/minuscole) + terminatore */
void d20_d12_d6(unsigned char *ascii, FILE *fp);

#endif
