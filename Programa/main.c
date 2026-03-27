#include <stdio.h>
#include <windows.h>
#include "menu.h"
#include "validacion.h"
#include "siteManagement.h"
#include "eventManagement.h"
#include "eventStatus.h"
#include "queryEvent.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    
    loadInitialSites();
    loadEvents();
    loadSegments();
    
    char user[50];
    char password[50];
    int tries = 5;
    int access = 0;
    
    for(;;) {
        int option1 = menuGeneral();
        switch (option1)
        {
        case 1:
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
                        printf("Dentro de lista de facturas\n");
                        break;
                    case 6:
                        printf("Dentro de estadísticas\n");
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
            printf("Dentro de compra de boletos\n");
            break;
        case 0:
            printf("Hasta la próxima\n");
            break;
        default:
            printf("Opción inválida\n");
            break;
        }
        if (option1 == 0) break;
    }
    return 0;
}