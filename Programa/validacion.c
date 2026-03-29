#include <stdio.h>
#include <string.h>
#include "validacion.h"

//Prototipos de funciones estáticas
static void cifrarCesar(char *texto, int shift);

/*
 * cifrarCesar
 *
 * Objetivo: Cifra un texto aplicando el algoritmo César con el desplazamiento indicado
 *
 * Entradas: char *texto - Cadena de texto a cifrar
 *           int shift - Valor de desplazamiento a aplicar en el cifrado
 *
 * Salidas: void
 *
 * Restricciones: texto no debe ser NULL
 */
static void cifrarCesar(char *texto, int shift) {
    for (int i = 0; texto[i] != '\0'; i++) {
        texto[i] = texto[i] + shift; //Desplaza el valor ASCII del carácter
    }
}

/*
 * leerCredenciales
 *
 * Objetivo: Abrir el archivo de credenciales y extraer el usuario y contraseña almacenados
 *
 * Entradas: char user[] - Arreglo donde se almacenará el usuario leído
 *           char password[] - Arreglo donde se almacenará la contraseña leída
 *
 * Salidas: void
 *
 * Restricciones: Se espera que el archivo exista en la ruta definida por RUTA_CREDENCIALES
 * y que tenga el formato JSON esperado
 */
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

/*
 * validate
 *
 * Objetivo: Validar las credenciales ingresadas por el usuario
 *
 * Entradas: char user[] - Usuario ingresado por el usuario
 *           char password[] - Contraseña ingresada por el usuario
 *
 * Salidas: int - Retorna 1 si las credenciales son correctas, 0 de lo contrario
 *
 * Restricciones: Se espera que el archivo de credenciales esté disponible y que
 * la contraseña almacenada esté cifrada con César
 */
int validate(char user[], char password[]) {
    char userFile[MAX_USUARIO];
    char passwordFile[MAX_PASSWORD];

    leerCredenciales(userFile, passwordFile); //Carga credenciales desde archivo

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