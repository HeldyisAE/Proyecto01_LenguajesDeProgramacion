#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "menu.h"
#include "validacion.h"
#include "siteManagement.h"
#include "eventManagement.h"
#include "eventStatus.h"
#include "queryEvent.h"
#include "billing.h"
#include "billing.h"
#include "ventaBoletos.h"

//Macros para el main
#define MAX_TRIES 5
#define MAX_USER 50
#define MAX_PASSWORD 50

/*
 * Main
 *
 * Objetivo: Gestiona el llamado de funciones para el funcionamiento correcto del programa
 *
 * Entradas: option1, option2, user, password - El usuario debe ingresar manualmente
 * estos valores para ingresar a las opciones de los menús
 *
 * Salidas: int - retorna 0 siguiendo el estandar del lenguaje
 *
 * Restricciones: Los input option1 y option 2 esperan valores numéricos dentro del rango esperado
 * respectivo para cada menú. Otro valor no es válido
 */
int main() {
    loadInitialSites();
    loadEvents();
    loadSegments();
    loadPrices();

    char user[MAX_USER];
    char password[MAX_PASSWORD];
    int tries = MAX_TRIES;
    int access = 0; // Toma valor = 1 en caso de que la función validate() valide las credenciales
    
    for(;;) {
        int option1 = menuGeneral();
        switch (option1)
        {
        case 1:
            access = 0;
            tries = 5;
            while (tries > 0 && !access) {
                printf("Ingrese su usuario: ");
                scanf("%49s", user);

                printf("Ingrese su contraseña: ");
                scanf("%49s", password);

                access = validate(user, password); //Valida las credenciales

                if (!access) {
                    printf("Credenciales incorrectas, intente de nuevo\n");
                    tries--;
                }
            }
            if(access) {

                for(;;) {
                    int option2 = menuAdmin();
                    switch (option2)
                    {
                    case 1:
                        manageSitesMenu(); //Menú para manejo de sitios
                        break;
                    case 2:
                        gestionarSectores(); //Menú para manejo de sectores en cada sitio
                        break;
                    case 3:
                        gestionarEventos(); //Menú para agregar eventos al sistema
                        break;
                    case 4:
                        printEventStatus(); //Consulta de estado de evento
                        break;
                    case 5:
                        mostrarListaFacturas(); //Consulta de facturas
                        break;
                    case 6:
                        mostrarEstadisticas();//Consulta de estadísticas
                        break;
                    case 0:
                        printf("Volviendo al menu principal...\n");
                        break;
                    default:
                        printf("Opcion invalida\n");
                        break;
                    }
                    if (option2 == 0) break;
                }
            } else {
                printf("Demasiados tries\n");
            }
            break;
        case 2:
            showEvents(); //Consulta de eventos - Público
            break;
        case 3:
            srand(time(NULL)); //Números aleatorios para la factura
            procesoCompra(); //Gestion de compra de boletos
            break;
        case 0:
            printf("--- HASTA LA PROXIMA ---\n");
            break;
        default:
            printf("Opcion invalida\n");
            break;
        }
        if (option1 == 0) break;
    }
    return 0;
}