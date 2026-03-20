#include <stdio.h>
#include <string.h>
#include "validacion.h"

void leerCredenciales(char user[], char password[]) {
    FILE *file = fopen("datos/credenciales.json", "r");

    if (file == NULL) {
        printf("Error al abrir archivo\n");
        return;
    }

    char buffer[200];
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
    char userFile[50];
    char passwordFile[50];

    leerCredenciales(userFile, passwordFile);

    if (strcmp(userFile, user) == 0 &&
        strcmp(passwordFile, password) == 0) {
        return 1;
    } else {
        printf("access denegado, intente de nuevo\n");
        return 0;
    }
}