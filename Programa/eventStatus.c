#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eventStatus.h"
#include "eventManagement.h"
#include "siteManagement.h"

/*
 * eventInfo
 *
 * Objetivo: Imprimir la información general de un evento específico
 *
 * Entradas: int index - Índice del evento dentro del arreglo eventos
 *
 * Salidas: void
 *
 * Restricciones: index debe ser un valor válido dentro del rango del arreglo de eventos.
 * Los arreglos de eventos y sitios debe estar inicializado
 */
void eventInfo(int index) {
    int sitioIndice = eventos[index].idSitio;

    printf("\n--- INFORMACION DEL EVENTO ---\n");
    printf("Nombre: %s\n", eventos[index].nombre);
    printf("Productora: %s\n", eventos[index].productora);
    printf("Fecha: %s\n", eventos[index].fecha);
    printf("Hora: %s\n", eventos[index].hora);
    printf("Sitio: %s\n", sitios[sitioIndice].nombre);
}

/*
 * sectorInfo
 *
 * Objetivo: Mostrar el estado de ocupación y recaudación de cada sector
 * para un evento específico
 *
 * Entradas: int index - Índice del evento dentro del arreglo global de eventos
 *
 * Salidas: void
 *
 * Restricciones: index debe ser un valor válido dentro del rango del arreglo de eventos.
 * Se espera que el archivo de facturas exista.
 */
void sectorInfo(int index) {
    int sitioIndice = eventos[index].idSitio;
    int numSect = sitios[sitioIndice].numSectores;

    printf("\n--- INFORMACION DE LOS SECTORES ---\n");

    for (int i = 0; i < numSect; i++) {
        struct Sector s = sitios[sitioIndice].sectores[i];

        float precio;

        // Obtiene los precios del arreglo
        if (eventos[index].preciosSectores != NULL) {
            precio = eventos[index].preciosSectores[i];
        } else {
            precio = s.precioBase;
        }

        // Crear arreglo de ocupación
        int *ocupados = (int *)calloc(s.capacidad, sizeof(int));
        if (!ocupados) {
            printf("Error de memoria\n");
            return;
        }

        float total = 0;

        // Leer archivo de ventas
        FILE *f = fopen("datos/facturas.txt", "r");
        if (f) {
            char linea[256];
            char nombreEv[100], nombreSec[50], cliente[50];
            int id, asiento;
            float precioVenta;

            while (fgets(linea, sizeof(linea), f)) {
                linea[strcspn(linea, "\n")] = 0;

                if (sscanf(linea, "%d|%49[^|]|%99[^|]|%49[^|]|%d|%f",
                           &id, cliente, nombreEv, nombreSec, &asiento, &precioVenta) == 6) {

                    // Verifica si coincide evento y sector
                    if (strcmp(nombreEv, eventos[index].nombre) == 0 &&
                        strcmp(nombreSec, s.nombre) == 0) {

                        if (asiento >= 1 && asiento <= s.capacidad) {
                            ocupados[asiento - 1] = 1;
                            total += precioVenta;
                        }
                    }
                }
            }
            fclose(f);
        }

        // Mostrar info del sector
        printf("\nSector: %s\n", s.nombre);
        printf("Precio por asiento: %.2f\n", precio);
        printf("Monto recaudado: %.2f\n", total);

        printf("Asientos:\n");

        for (int j = 0; j < s.capacidad; j++) {
            if (ocupados[j]) {
                printf("%c%d - Ocupado\n", s.codigoInicial, j + 1);
            } else {
                printf("%c%d - Disponible\n", s.codigoInicial, j + 1);
            }
        }

        free(ocupados);
    }
}

/*
 * printEventStatus
 *
 * Objetivo: Mostrar el menú de selección de eventos
 *
 * Entradas: option - Valor ingresado por el usuario para seleccionar el evento
 *
 * Salidas: void
 *
 * Restricciones: Debe haber al menos un evento registrado en el sistema.
 * Se espera que el usuario ingrese un valor numérico dentro del rango válido
 */
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
    if (scanf("%d", &option) != 1) {
        printf("Entrada invalida.\n");
        while (getchar() != '\n');
        return;
    }

    if (option < 1 || option > numEventos) {
        printf(" ! Seleccion invalida.\n");
        return;
    }

    int index = option - 1;

    eventInfo(index);
    sectorInfo(index);
}