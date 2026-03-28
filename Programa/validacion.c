#include <stdio.h>
#include <string.h>
#include "validacion.h"

static void cifrarCesar(char *texto, int shift);

static void cifrarCesar(char *texto, int shift) {
    for (int i = 0; texto[i] != '\0'; i++) {
        texto[i] = texto[i] + shift;
    }
}

void leerCredenciales(char user[], char password[]) {
    FILE *file = fopen(RUTA_CREDENCIALES, "r");

    if (file == NULL) {
        printf("Error al abrir archivo\n");
        return;
    }

    char buffer[SIZE_BUFFER_CREDENCIALES];
    size_t bytesLeidos = fread(buffer, sizeof(char), sizeof(buffer) - 1, file);
    buffer[bytesLeidos] = '\0';

    fclose(file);

    char *ptrUser = strstr(buffer, "\"usuario\": \"");
    if (ptrUser != NULL) {
        ptrUser += strlen("\"usuario\": \"");
        sscanf(ptrUser, "%[^\"]", user);
    }

    char *ptrPassword = strstr(buffer, "\"password\": \"");
    if (ptrPassword != NULL) {
        ptrPassword += strlen("\"password\": \"");
        sscanf(ptrPassword, "%[^\"]", password);
    }
}

int validate(char user[], char password[]) {
    char userFile[MAX_USUARIO];
    char passwordFile[MAX_PASSWORD];

    leerCredenciales(userFile, passwordFile);

    // Copia de la contraseña ingresada
    char passwordCifrada[50];
    strcpy(passwordCifrada, password);

    cifrarCesar(passwordCifrada, 3);

    if (strcmp(userFile, user) == 0 &&
        strcmp(passwordFile, passwordCifrada) == 0) {
        return 1;
    } else {
        printf("access denegado, intente de nuevo\n");
        return 0;
    }
}