#include <stdio.h>
#include "menu.h"

/*
 * MenuAdmin
 *
 * Objetivo: Imprimir el menú de opciones de administrador
 *
 * Entradas: option - Representa la elección del usuario dentro del menu
 *
 * Salidas: option - Retorna la opción para ser utilizada en main
 *
 * Restricciones: Se espera que el usuario ingrese un int dentro del
 * rango correspondiente
 */
int menuAdmin() {
    int option;

    printf("------------------------------\n");
    printf("     MENU ADMINISTRATIVO\n");
    printf("------------------------------\n");
    printf("Seleccione una opcion:\n");
    printf("------------------------------\n");
    printf("1. Gestion de Sitios de Eventos\n");
    printf("2. Gestion de espacios sitio de eventos\n");
    printf("3. Gestion de Eventos\n");
    printf("4. Estado de evento\n");
    printf("5. Lista de facturas\n");
    printf("6. Estadisticas\n");
    printf("0. Volver\n");
    printf("------------------------------\n");
    printf("> ");

    while (scanf("%d", &option) != 1) {
        while (getchar() != '\n'); //Limpia el buffer si la entrada no es válida
        printf("Opción no válida, intente de nuevo: ");
    }
    while (getchar() != '\n'); //Limpia el salto de línea restante

    return option;
}

/*
 * menuGeneral
 *
 * Objetivo: Imprimir el menú de opciones generales
 *
 * Entradas: option - Representa la elección del usuario dentro del menu
 *
 * Salidas: option - Retorna la opción para ser utilizada en main
 *
 * Restricciones: Se espera que el usuario ingrese un int dentro del
 * rango correspondiente
 */
int menuGeneral() {
    int option;

    printf("------------------------------\n");
    printf("        e-VENTOS\n");
    printf("------------------------------\n");
    printf("Le damos la bienvenida\n");
    printf("------------------------------\n");
    printf("Seleccione una opcion:\n");
    printf("------------------------------\n");
    printf("1. Opciones administrativas\n");
    printf("2. Consulta por evento\n");
    printf("3. Compra de boletos\n");
    printf("0. Salir\n");
    printf("------------------------------\n");
    printf("> ");

    while (scanf("%d", &option) != 1) {
        while (getchar() != '\n'); //Limpia el buffer si la entrada no es válida
        printf("Opción no válida, intente de nuevo: ");
    }
    while (getchar() != '\n'); //Limpia el salto de línea restante

    return option;
}