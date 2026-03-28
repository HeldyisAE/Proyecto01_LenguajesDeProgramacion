#ifndef VALIDATE_H
#define VALIDATE_H

#define RUTA_CREDENCIALES  "datos/credenciales.json"
#define SIZE_BUFFER_CREDENCIALES  200
#define MAX_USUARIO   50
#define MAX_PASSWORD  50

int validate(char user[], char password[]);
void leerCredenciales(char user[], char password[]);

#endif