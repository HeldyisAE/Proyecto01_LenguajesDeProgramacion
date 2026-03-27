#include <stdio.h>
#include <string.h>
#include "eventManagement.h"
#include "siteManagement.h"
#include "queryEvent.h"

static int fechaToInt(char *f);

static void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static int fechaToInt(char *f) {
    int d, m, a;
    sscanf(f, "%d/%d/%d", &d, &m, &a);
    return a * 10000 + m * 100 + d;
}

void showEvents() {
    if (numEventos == 0) {
        printf("\nNo hay eventos registrados.\n");
        return;
    }

    char fechaFiltro[15];

    limpiarBuffer();
    printf("\nIngrese la fecha inicial (DD/MM/AAAA): ");
    fgets(fechaFiltro, sizeof(fechaFiltro), stdin);
    fechaFiltro[strcspn(fechaFiltro, "\n")] = 0;

    int d, m, a;
    if (sscanf(fechaFiltro, "%d/%d/%d", &d, &m, &a) != 3) {
        printf("Formato de fecha invalido.\n");
        return;
    }

    printf("\n--- EVENTOS DISPONIBLES ---\n");

    int indices[100];
    int count = 0;

    // Filtro de eventos por punto de fecha en adelante
    for (int i = 0; i < numEventos; i++) {

        if (fechaToInt(eventos[i].fecha) >= fechaToInt(fechaFiltro)) {
            printf("%d. %s - %s\n", count + 1,
                   eventos[i].nombre,
                   eventos[i].fecha);

            indices[count] = i;
            count++;
        }
    }

    if (count == 0) {
        printf("\nNo hay eventos futuros.\n");
        return;
    }

    int sel;
    printf("\nSeleccione un evento: ");
    if (scanf("%d", &sel) != 1 || sel < 1 || sel > count) {
        printf("Seleccion invalida.\n");
        limpiarBuffer();
        return;
    }
    limpiarBuffer();

    int idx = indices[sel - 1];
    struct Evento e = eventos[idx];

    if (e.idSitio < 0 || e.idSitio >= numSitios) {
        printf("Error: sitio invalido.\n");
        return;
    }
    struct Site s = sitios[e.idSitio];

    printf("\n========== DETALLE DEL EVENTO ==========\n");
    printf("Nombre      : %s\n", e.nombre);
    printf("Productora  : %s\n", e.productora);
    printf("Sitio       : %s\n", s.nombre);
    printf("Fecha       : %s\n", e.fecha);
    printf("Hora        : %s\n", e.hora);

    printf("\n--- Sectores ---\n");

    for (int i = 0; i < s.numSectores; i++) {

        float precio;

        if (e.preciosSectores != NULL) {
            precio = e.preciosSectores[i];
        } else {
            precio = s.sectores[i].precioBase;
        }

        printf("\nSector: %s\n", s.sectores[i].nombre);
        printf("Precio por asiento: %.2f\n", precio);

        // FALTA LOGICA DE COMPRA
        printf("Asientos disponibles: %d\n",
               s.sectores[i].capacidad);
    }

    printf("\n=======================================\n");
}