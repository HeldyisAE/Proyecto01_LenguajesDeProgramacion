#include <stdio.h>
#include <windows.h>
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

#define MAX_TRIES 5
#define MAX_USER 50
#define MAX_PASSWORD 50

int main() {
    SetConsoleOutputCP(CP_UTF8);
    
    loadInitialSites();
    loadEvents();
    loadSegments();
    loadPrices();

    char user[MAX_USER];
    char password[MAX_PASSWORD];
    int tries = MAX_TRIES;
    int access = 0;
    
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

                access = validate(user, password);

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
                        manageSitesMenu();
                        break;
                    case 2:
                        gestionarSectores(); 
                        break;
                    case 3:
                        gestionarEventos();
                        break;
                    case 4:
                        printEventStatus();
                        break;
                    case 5:
                        mostrarListaFacturas();
                        break;
                    case 6:
                        mostrarEstadisticas();
                        break;
                    case 0:
                        printf("Volviendo al menú principal...\n");
                        break;
                    default:
                        printf("Opción inválida\n");
                        break;
                    }
                    if (option2 == 0) break;
                }
            } else {
                printf("Demasiados tries\n");
            }
            break;
        case 2:
            showEvents();
            break;
        case 3:
            srand(time(NULL));
            procesoCompra();
            break;
        case 0:
            printf("--- HASTA LA PRÓXIMA ---\n");
            break;
        default:
            printf("Opción inválida\n");
            break;
        }
        if (option1 == 0) break;
    }
    return 0;
}