#include <stdio.h>
#include "eventStatus.h"
#include "eventManagement.h"

void eventInfo(int index) {
    int sitioIndice = eventos[index].idSitio;

    printf("\n--- INFORMACION DEL EVENTO ---\n");
    printf("Nombre: %s\n", eventos[index].nombre);
    printf("Productora: %s\n", eventos[index].productora);
    printf("Fecha: %s\n", eventos[index].fecha);
    printf("Hora: %s\n", eventos[index].hora);
    printf("Sitio: %s\n", sitios[sitioIndice].nombre);
}

void sectorInfo(int index) {
    int sitioIndice = eventos[index].idSitio;
    int numSect = sitios[sitioIndice].numSectores;

    printf("\n--- INFORMACIÓN DE LOS SECTORES ---\n");

    for (int i = 0; i < numSect; i++) {
        struct Sector s = sitios[sitioIndice].sectores[i];

        float precio;

        if (eventos[index].preciosSectores != NULL) {
            precio = eventos[index].preciosSectores[i];
        } else {
            precio = s.precioBase;
        }

        printf("\nSector: %s\n", s.nombre);
        printf("Precio por asiento: %.2f\n", precio);

        // Aun no hay ventas
        printf("Monto recaudado: 0.00\n");

        printf("Asientos:\n");

        //Todo está disponible porque aun no hay lógica de venta
        for (int j = 0; j < s.capacidad; j++) {
            printf("%c%d - Disponible\n", s.codigoInicial, j + 1);
        }
        
    }
}

void printEventStatus() {
    if (numEventos == 0) {
        printf("\n ! No hay eventos registrados.\n");
        return;
    }

    printf("\n--- ESTADO DE EVENTOS --- \n");

    for (int i = 0; i < numEventos; i++) {
        printf("%d. %s\n", i + 1, eventos[i].nombre);
    }

    int option;
    printf("Seleccione un evento: ");
    scanf("%d", &option);

    if (option < 1 || option > numEventos) {
        printf(" ! Seleccion invalida.\n");
        return;
    }

    int index = option - 1;

    eventInfo(index);
    sectorInfo(index);
}
