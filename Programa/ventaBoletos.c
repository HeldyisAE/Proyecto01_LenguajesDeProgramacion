#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ventaBoletos.h"
#include "eventManagement.h"
#include "siteManagement.h"

void procesoCompra() {
    if (numEventos == 0) {
        printf("\n ! No hay eventos disponibles.\n");
        return;
    }

    printf("\n--- EVENTOS DISPONIBLES ---\n");
    for (int i = 0; i < numEventos; i++) {
        printf("%d. %s\n", i + 1, eventos[i].nombre);
    }

    int selEv;
    printf("Seleccione un evento: ");
    scanf("%d", &selEv);
    int evIdx = selEv - 1;
    int sitIdx = eventos[evIdx].idSitio;

    printf("\nSECTORES en %s:\n", sitios[sitIdx].nombre);
    for (int j = 0; j < sitios[sitIdx].numSectores; j++) {

        printf("%d. %s (Precio: %.2f)\n", j + 1, sitios[sitIdx].sectores[j].nombre, eventos[evIdx].preciosSectores[j]);
    }

    int selSec;
    printf("Seleccione sector: ");
    if (scanf("%d", &selSec) != 1) {
        printf("Entrada invalida. Debe ingresar un numero.\n");
        while (getchar() != '\n'); 
        return;
    }
    int secIdx = selSec - 1;

    char nombreC[50];
    printf("\nNombre del cliente: ");
    getchar(); 
    fgets(nombreC, 50, stdin);
    nombreC[strcspn(nombreC, "\n")] = 0;

    int asiento;
    printf("\nNumero de asiento (1 a %d): ", sitios[sitIdx].sectores[secIdx].capacidad);
    scanf("%d", &asiento);


    FILE *f = fopen("datos/facturas.txt", "a");
    if (f) {
        int id = rand() % 9000 + 1000;
        fprintf(f, "%d|%s|%s|%s|%d|%.2f\n", 
                id, nombreC, eventos[evIdx].nombre, 
                sitios[sitIdx].sectores[secIdx].nombre, asiento, 
                eventos[evIdx].preciosSectores[secIdx]);
        fclose(f);
        printf("\n[OK] Venta registrada. Factura #%d generada.\n", id);
    }
}