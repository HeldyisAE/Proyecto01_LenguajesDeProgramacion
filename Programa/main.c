#include <stdio.h>
#include <windows.h>
#include "menu.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    
    for(;;) {
        int option1 = menuGeneral();
        switch (option1)
        {
        case 1:
            for(;;) {
                int option2 = menuAdmin();
                switch (option2)
                {
                case 1:
                    printf("Dentro de gestor de sitios de eventos\n");
                    break;
                case 2:
                    printf("Dentro de gestor de espacios en sitio de evento\n");
                    break;
                case 3:
                    printf("Dentro de gestor de eventos\n");
                    break;
                case 4:
                    printf("Dentro de estado de evento\n");
                    break;
                case 5:
                    printf("Dentro de lista de facturas\n");
                    break;
                case 6:
                    printf("Dentro de estadísticas\n");
                    break;
                case 7:
                    printf("Volviendo al menú principal...\n");
                    break;
                default:
                    printf("Opción inválida\n");
                    break;
                }
                if (option2 == 7) break;
            }
            break;
        case 2:
            printf("Dentro de consulta por eventos\n");
            break;
        case 3:
            printf("Dentro de compra de boletos\n");
            break;
        case 4:
            printf("Hasta la próxima\n");
            break;
        default:
            printf("Opción inválida\n");
            break;
        }
        if (option1 == 4) break;
    }
    return 0;
}