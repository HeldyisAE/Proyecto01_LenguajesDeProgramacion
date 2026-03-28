#ifndef VALIDATE_H
#define VALIDATE_H

//Definición de macros
#define RUTA_CREDENCIALES  "datos/credenciales.json"
#define SIZE_BUFFER_CREDENCIALES  200
#define MAX_USUARIO   50
#define MAX_PASSWORD  50

//Prototipos
int validate(char user[], char password[]);
void leerCredenciales(char user[], char password[]);

#endif