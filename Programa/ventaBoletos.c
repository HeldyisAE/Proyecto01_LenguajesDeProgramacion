#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ventaBoletos.h"
#include "eventManagement.h"
#include "siteManagement.h"

//Prototipos de funciones estáticas
static void obtenerFechaActual(char *buffer);
static int asientoYaVendido(char *nombreEvento, char *nombreSector, int asiento);

/*
 * obtenerFechaActual
 *
 * Objetivo: Obtener la fecha actual del sistema
 *
 * Entradas: char *buffer - Arreglo donde se almacenará la fecha formateada
 *
 * Salidas: void
 *
 * Restricciones: El buffer debe tener espacio suficiente
 */
static void obtenerFechaActual(char *buffer) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    strftime(buffer, 15, "%d/%m/%Y", tm);
}

/*
 * asientoYaVendido
 *
 * Objetivo: Verificar si un asiento ya fue vendido para un evento y sector específico
 *
 * Entradas: char *nombreEvento - Nombre del evento a consultar
 *           char *nombreSector - Nombre del sector a consultar
 *           int asiento - Número del asiento a verificar
 *
 * Salidas: int - Retorna 1 si el asiento está ocupado, 0 si está libre
 *
 * Restricciones: Se espera que el archivo de facturas exista
 */
static int asientoYaVendido(char *nombreEvento, char *nombreSector, int asiento) {
    FILE *f = fopen("datos/facturas.txt", "r");
    if (!f) return 0; //0 es libre, 1 es ocupado

    char linea[MAX_LINEA_FACTURA];
    int id, asientoFile;
    char cliente[MAX_NOMBRE_CLIENTE], evento[MAX_NOMBRE_EVENTO], sector[MAX_NOMBRE_SECTOR], fecha[MAX_FECHA];
    float precio;

    while (fgets(linea, sizeof(linea), f)) {
        linea[strcspn(linea, "\n")] = 0;

        if (sscanf(linea, "%d|%49[^|]|%99[^|]|%49[^|]|%d|%f|%14[^|]",
                   &id, cliente, evento, sector, &asientoFile, &precio, fecha) == 7) {

            //Comparaciones
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

/*
 * procesoCompra
 *
 * Objetivo: Gestionar el proceso completo de compra de boletos
 *
 * Entradas: Los valores son ingresados manualmente por el usuario
 * durante la ejecución
 *
 * Salidas: void
 *
 * Restricciones: Debe haber al menos un evento registrado en el sistema.
 * Los índices de evento y sector deben estar dentro del rango válido.
 * La cantidad de boletos no debe superar MAX_ASIENTOS
 */
void procesoCompra() {
    if (numEventos == 0) {
        printf("\n! No hay eventos disponibles.\n");
        return;
    }

    printf("\n--- EVENTOS DISPONIBLES ---\n");
    for (int i = 0; i < numEventos; i++) {
        printf("%d. %s\n", i + 1, eventos[i].nombre);
    }

    // Validación de selección de evento
    int selEv;
    printf("Seleccione un evento: ");
    while (scanf("%d", &selEv) != 1 || selEv < 1 || selEv > numEventos) {
        while (getchar() != '\n'); //Limpia el buffer si la entrada no es válida
        printf("Opción invalida, intente de nuevo: ");
    }
    int evIndex = selEv - 1; //Resta 1 a la selección para obtener el indice real
    int sitIndex = eventos[evIndex].idSitio;

    printf("\nSECTORES en %s:\n", sitios[sitIndex].nombre);
    for (int j = 0; j < sitios[sitIndex].numSectores; j++) {
        printf("%d. %s (Precio: %.2f)\n",
               j + 1,
               sitios[sitIndex].sectores[j].nombre,
               eventos[evIndex].preciosSectores[j]);
    }

    // Validación de selección de sector
    int selSec;
    printf("Seleccione sector: ");
    while (scanf("%d", &selSec) != 1 || selSec < 1 || selSec > sitios[sitIndex].numSectores) {
        while (getchar() != '\n'); //Limpia el buffer si la entrada no es válida
        printf("Opción invalida, intente de nuevo: ");
    }
    int secIndex = selSec - 1;

    // Validación de nombre del cliente
    char nombreC[MAX_NOMBRE_CLIENTE];
    getchar();
    do {
        printf("\nNombre del cliente: ");
        fgets(nombreC, MAX_NOMBRE_CLIENTE, stdin);
        nombreC[strcspn(nombreC, "\n")] = 0;

        if (strlen(nombreC) == 0) {
            printf("El nombre no puede estar vacío.\n");
        }
    } while (strlen(nombreC) == 0);

    // Validación de cédula
    char cedula[MAX_CEDULA];
    do {
        printf("Cedula: ");
        if (scanf("%19s", cedula) != 1) {
            while (getchar() != '\n'); //Limpia el buffer si la entrada no es válida
            cedula[0] = '\0';
            printf("Cedula invalida.\n");
        }
    } while (strlen(cedula) == 0);

    // Validación de cantidad de boletos
    int cantidad;
    printf("\nCuantos boletos desea comprar?: ");
    while (scanf("%d", &cantidad) != 1 || cantidad < 1 || cantidad > MAX_ASIENTOS) {
        while (getchar() != '\n'); //Limpia el buffer si la entrada no es válida
        printf("Cantidad invalida (1 a %d): ", MAX_ASIENTOS);
    }

    int asientos[MAX_ASIENTOS];
    float precio = eventos[evIndex].preciosSectores[secIndex]; //Obtiene precio del arreglo preciosSectores
    float subtotal = 0;

    for (int i = 0; i < cantidad; i++) {
        // Validación de número de asiento
        printf("Asiento #%d (1 a %d): ", i + 1, sitios[sitIndex].sectores[secIndex].capacidad);
        while (scanf("%d", &asientos[i]) != 1 ||
               asientos[i] < 1 ||
               asientos[i] > sitios[sitIndex].sectores[secIndex].capacidad) {
            while (getchar() != '\n'); //Limpia el buffer si la entrada no es válida
            printf("Asiento invalido (1 a %d): ", sitios[sitIndex].sectores[secIndex].capacidad);
        }

        if (asientoYaVendido(eventos[evIndex].nombre, sitios[sitIndex].sectores[secIndex].nombre, asientos[i])) {
            printf("El asiento %d ya fue vendido.\n", asientos[i]);
            return;
        }

        subtotal += precio;
    }

    //Cálculo de cobro
    float servicio = subtotal * 0.05;
    float total = subtotal + servicio;

    char fechaCompra[MAX_FECHA];
    obtenerFechaActual(fechaCompra);

    int id = rand() % 9000 + 1000; //Genera aleatorio de la factura

    //Append de la nueva compra
    FILE *f = fopen("datos/facturas.txt", "a");
    if (f) {
        for (int i = 0; i < cantidad; i++) {
            fprintf(f, "%d|%s|%s|%s|%d|%.2f|%s\n",
                    id,
                    nombreC,
                    eventos[evIndex].nombre,
                    sitios[sitIndex].sectores[secIndex].nombre,
                    asientos[i],
                    precio,
                    fechaCompra);
        }
        fclose(f);
    }

    //Imprime detalles de factura
    printf("\n=========== FACTURA ===========\n");
    printf("ID Factura: %d\n", id);
    printf("Fecha compra: %s\n", fechaCompra);
    printf("Cedula: %s\n", cedula);
    printf("Cliente: %s\n", nombreC);
    printf("Evento: %s\n", eventos[evIndex].nombre);
    printf("Productora: %s\n", eventos[evIndex].productora);
    printf("Sitio: %s\n", sitios[sitIndex].nombre);
    printf("Fecha evento: %s\n", eventos[evIndex].fecha);

    printf("\n--- DETALLE ---\n");
    for (int i = 0; i < cantidad; i++) {
        printf("Asiento %d - %.2f\n", asientos[i], precio);
    }

    printf("\nSubtotal: %.2f\n", subtotal);
    printf("Servicio (5%%): %.2f\n", servicio);
    printf("TOTAL: %.2f\n", total);
    printf("================================\n");
}