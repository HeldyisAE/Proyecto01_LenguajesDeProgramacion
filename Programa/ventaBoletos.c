#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ventaBoletos.h"
#include "eventManagement.h"
#include "siteManagement.h"

static void obtenerFechaActual(char *buffer);
static int asientoYaVendido(char *nombreEvento, char *nombreSector, int asiento);

static void obtenerFechaActual(char *buffer) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    strftime(buffer, 15, "%d/%m/%Y", tm);
}

static int asientoYaVendido(char *nombreEvento, char *nombreSector, int asiento) {
    FILE *f = fopen("datos/facturas.txt", "r");
    if (!f) return 0; //0 es libre, 1 es ocupado

    char linea[256];
    int id, asientoFile;
    char cliente[50], evento[100], sector[50], fecha[15];
    float precio;

    while (fgets(linea, sizeof(linea), f)) {
        linea[strcspn(linea, "\n")] = 0;

        if (sscanf(linea, "%d|%49[^|]|%99[^|]|%49[^|]|%d|%f|%14[^|]",
                   &id, cliente, evento, sector, &asientoFile, &precio, fecha) == 7) {

            if (strcmp(evento, nombreEvento) == 0 &&
                strcmp(sector, nombreSector) == 0 &&
                asientoFile == asiento) {

                fclose(f);
                return 1; 
            }
        }
    }

    fclose(f);
    return 0; 
}

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

    // Validación de rango
    if (asiento < 1 || asiento > sitios[sitIdx].sectores[secIdx].capacidad) {
        printf("Asiento invalido.\n");
        return;
    }

    // Validación de venta
    if (asientoYaVendido(
            eventos[evIdx].nombre,
            sitios[sitIdx].sectores[secIdx].nombre,
            asiento)) {

        printf("El asiento ya fue vendido.\n");
        return;
    }

    char fecha[15];
    obtenerFechaActual(fecha);

    FILE *f = fopen("datos/facturas.txt", "a");
    if (f) {
        int id = rand() % 9000 + 1000;
        fprintf(f, "%d|%s|%s|%s|%d|%.2f|%s\n",
                id,
                nombreC,
                eventos[evIdx].nombre,
                sitios[sitIdx].sectores[secIdx].nombre,
                asiento,
                eventos[evIdx].preciosSectores[secIdx],
                fecha);
        fclose(f);
        printf("\n[OK] Venta registrada. Factura #%d generada.\n", id);
    }
}