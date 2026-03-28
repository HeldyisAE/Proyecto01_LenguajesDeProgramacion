#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eventManagement.h"
#include "siteManagement.h"

//Definición de variables globales
struct Evento *eventos = NULL;
int numEventos = 0;

//Prototipos de funciones estáticas
static void limpiarBuffer();
static void cargarEventosDesdeArchivo(const char *rutaArchivo);
static void cargarSectoresDesdeArchivo(const char *rutaArchivo);

/*
 * limpiarBuffer
 *
 * Objetivo: Libera buffer después de un scanf
 *
 * Entradas: Ninguna
 *
 * Salidas: void
 *
 * Restricciones: Sin restricciones específicas
 */
static void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/*
 * generarAsientos
 *
 * Objetivo: Generar y asignar los identificadores de asientos para un sector
 *
 * Entradas: struct Sector *sec - Puntero al sector al que se le generarán los asientos
 *
 * Salidas: void
 *
 * Restricciones: sec no debe ser NULL. sec->capacidad debe ser mayor a 0
 */
void generarAsientos(struct Sector *sec) {
    sec->asientos = (char **)malloc(sec->capacidad * sizeof(char*)); //Accede los asientos

    for (int i = 0; i < sec->capacidad; i++) {
        sec->asientos[i] = (char *)malloc(10 * sizeof(char));
        sprintf(sec->asientos[i], "%c%d", sec->codigoInicial, i + 1); //Escribe los asientos en el arreglo
    }
}

/*
 * liberarAsientos
 *
 * Objetivo: Liberar la memoria dinámica asignada a los asientos de un sector
 *
 * Entradas: struct Sector *sec - Puntero al sector cuyos asientos se liberarán
 *
 * Salidas: void
 *
 * Restricciones: sec no debe ser NULL. Los asientos deben haber sido generados
 * previamente con generarAsientos
 */
void liberarAsientos(struct Sector *sec) {
    for (int i = 0; i < sec->capacidad; i++) {
        free(sec->asientos[i]);
    }
    free(sec->asientos);
}

/*
 * gestionarSectores
 *
 * Objetivo: Mostrar el menú de gestión de sectores y dirigir al usuario
 * a la opción seleccionada
 *
 * Entradas: opcion - Valor ingresado por el usuario para elegir una opción
 *
 * Salidas: void
 *
 * Restricciones: Se espera que el usuario ingrese un valor numérico dentro
 * del rango válido
 */
void gestionarSectores() {
    int opcion;

    do {
        printf("\n--- GESTION DE SECTORES ---\n");
        printf("1. Ver sitios y sectores\n");
        printf("2. Agregar sector a sitio\n");
        printf("3. Resetear sectores de un sitio\n");
        printf("0. Volver\n");
        printf("> ");
        scanf("%d", &opcion);
        limpiarBuffer();

        switch (opcion) {
            case 1:
                mostrarSitiosYSectores();
                break;
            case 2:
                agregarSectorASitio();
                break;
            case 3:
                resetearSectores();
                break;
        }

    } while (opcion != 0);
}

/*
 * mostrarSitiosYSectores
 *
 * Objetivo: Recorrer e imprimir todos los sitios con sus sectores y asientos
 *
 * Entradas: Ninguna
 *
 * Salidas: void
 *
 * Restricciones: El arreglo global de sitios debe estar inicializado
 */
void mostrarSitiosYSectores() {
    for (int i = 0; i < numSitios; i++) {
        printf("\nSitio: %s\n", sitios[i].nombre);

        if (sitios[i].numSectores == 0) {
            printf("  (Sin sectores)\n");
            continue;
        }

        for (int j = 0; j < sitios[i].numSectores; j++) {
            struct Sector *sec = &sitios[i].sectores[j];

            printf("  Sector: %s | Capacidad: %d | Precio: %.2f\n",
                   sec->nombre, sec->capacidad, sec->precioBase);

            printf("    Asientos: ");
            for (int k = 0; k < sec->capacidad; k++) {
                printf("\n    %s ", sec->asientos[k]);
                printf("\n");
            }
            printf("\n");
        }
    }
}

/*
 * agregarSectorASitio
 *
 * Objetivo: Solicitar los datos de uno o más sectores nuevos y agregarlos
 * al sitio seleccionado
 *
 * Entradas: sel - Índice del sitio seleccionado por el usuario
 *           sectoresToAdd - Cantidad de sectores a agregar
 *
 * Salidas: void
 *
 * Restricciones: Debe haber al menos un sitio registrado en el sistema.
 * sel debe ser un valor válido dentro del rango de sitios disponibles
 */
void agregarSectorASitio() {
    if (numSitios == 0) {
        printf("No hay sitios.\n");
        return;
    }

    for (int i = 0; i < numSitios; i++) {
        printf("%d. %s\n", i + 1, sitios[i].nombre);
    }

    int sel;
    printf("\nSeleccione el sitio: ");
    scanf("%d", &sel);
    limpiarBuffer();

    int idx = sel - 1;
    struct Site *s = &sitios[idx];

    struct Sector *temp = realloc(s->sectores, (s->numSectores + 1) * sizeof(struct Sector));

    if (!temp) return; //Termina si el puntero está vacío
    s->sectores = temp; //Intercambia valores

    int sectoresToAdd;
    printf("\n¿Cuántos sectores desea agregar?: ");
    scanf("%d", &sectoresToAdd);
    limpiarBuffer();

    for (int i = 0; i < sectoresToAdd; i++) {
        struct Sector *nuevo = &s->sectores[s->numSectores];

        printf("\n--- Sector %d ---\n", i + 1);

        printf("\nNombre sector: ");
        fgets(nuevo->nombre, MAX_NOMBRE_SECTOR, stdin);
        nuevo->nombre[strcspn(nuevo->nombre, "\n")] = 0;

        printf("\nInicial identificadora: ");
        scanf(" %c", &nuevo->codigoInicial);

        printf("\nCapacidad: ");
        scanf("%d", &nuevo->capacidad);

        printf("\nPrecio base: ");
        scanf("%f", &nuevo->precioBase);
        limpiarBuffer();

        generarAsientos(nuevo);
        s->numSectores++;
        guardarSectores();

        printf("Sector agregado correctamente.\n");
    }
}

/*
 * resetearSectores
 *
 * Objetivo: Eliminar todos los sectores de un sitio seleccionado
 *
 * Entradas: sel - Índice del sitio seleccionado por el usuario
 *
 * Salidas: void
 *
 * Restricciones: sel debe ser un valor válido dentro del rango de sitios disponibles
 */
void resetearSectores() {
    for (int i = 0; i < numSitios; i++) {
        printf("%d. %s\n", i + 1, sitios[i].nombre);
    }

    int sel;
    printf("Seleccione sitio: ");
    scanf("%d", &sel);
    limpiarBuffer();

    int idx = sel - 1;
    struct Site *s = &sitios[idx];

    for (int i = 0; i < s->numSectores; i++) {
        liberarAsientos(&s->sectores[i]);
    }

    free(s->sectores); //Libera la memoria del arreglo
    s->sectores = NULL;
    s->numSectores = 0;

    guardarSectores(); //Guarda el nuevo estado en el archivo
    printf("Sectores eliminados.\n");
}

/*
 * gestionarEventos
 *
 * Objetivo: Solicitar los datos de un nuevo evento y registrarlo en el sistema
 *
 * Entradas: Ninguna - Los valores son ingresados manualmente por el usuario
 * durante la ejecución
 *
 * Salidas: void
 *
 * Restricciones: Debe haber al menos un sitio registrado en el sistema.
 * El sitio seleccionado debe tener sectores configurados para poder
 * asignar precios
 */
void gestionarEventos() {
    if (numSitios == 0) {
        printf("\n ! No se pueden crear eventos sin sitios.\n");
        return;
    }

    printf("\n--- REGISTRAR NUEVO EVENTO ---\n");

    struct Evento *temp = (struct Evento *)realloc(eventos, (numEventos + 1) * sizeof(struct Evento));
    //Verifica que el arreglo no esté vacío
    if (temp == NULL) {
        printf(" ! Error de memoria.\n");
        return;
    }
    eventos = temp; //Intercambia contenido

    limpiarBuffer();

    printf("Nombre del Evento: ");
    fgets(eventos[numEventos].nombre, MAX_NOMBRE_EVENTO, stdin);
    eventos[numEventos].nombre[strcspn(eventos[numEventos].nombre, "\n")] = 0;

    printf("Productora: ");
    fgets(eventos[numEventos].productora, MAX_PRODUCTORA, stdin);
    eventos[numEventos].productora[strcspn(eventos[numEventos].productora, "\n")] = 0;

    printf("Fecha (DD/MM/AAAA): ");
    scanf("%14s", eventos[numEventos].fecha);

    printf("Hora (HH:MM): ");
    scanf("%9s", eventos[numEventos].hora);
    limpiarBuffer();

    printf("\nSeleccione el Lugar:\n");
    for (int i = 0; i < numSitios; i++) {
        printf("%d. %s\n", i + 1, sitios[i].nombre);
    }

    int sel;
    if (scanf("%d", &sel) != 1 || sel < 1 || sel > numSitios) {
        printf(" ! Seleccion invalida.\n");
        limpiarBuffer();
        return;
    }
    limpiarBuffer();

    int sIdx = sel - 1;
    eventos[numEventos].idSitio = sIdx;

    int nSect = sitios[sIdx].numSectores;

    if (nSect <= 0 || sitios[sIdx].sectores == NULL) {
        printf("\n ! El sitio no tiene sectores configurados.\n");
        eventos[numEventos].preciosSectores = NULL;
    } else {
        eventos[numEventos].preciosSectores = (float *)malloc(nSect * sizeof(float));

        if (eventos[numEventos].preciosSectores == NULL) {
            printf(" ! Error de memoria.\n");
            return;
        }

        printf("\n--- Ajuste de Precios para este Evento ---\n");

        for (int i = 0; i < nSect; i++) {
            printf("  Precio para %s (Base: %.2f): ",
                sitios[sIdx].sectores[i].nombre,
                sitios[sIdx].sectores[i].precioBase);
            scanf("%f", &eventos[numEventos].preciosSectores[i]);
        }
        limpiarBuffer();
    }

    numEventos++;
    printf("\nEvento registrado.\n");
    guardarEventos();
    savePrices();
}

/*
 * cargarEventosDesdeArchivo
 *
 * Objetivo: Abrir el archivo de eventos y cargar su contenido al arreglo
 *
 * Entradas: const char *rutaArchivo - Ruta del archivo a leer
 *
 * Salidas: void
 *
 * Restricciones: Se espera que el archivo exista y tenga el formato
 * nombre|productora|fecha|hora|idSitio
 */
static void cargarEventosDesdeArchivo(const char *rutaArchivo) {
    FILE *f = fopen(rutaArchivo, "r");
    if (!f) {
        printf("No se pudo abrir eventos.\n");
        return;
    }

    char linea[MAX_LINEA];
    char nombre[MAX_NOMBRE_EVENTO], productora[MAX_PRODUCTORA];
    char fecha[MAX_FECHA_EVENTO], hora[MAX_HORA_EVENTO];
    int idSitio;

    while (fgets(linea, sizeof(linea), f)) {
        if (linea[strlen(linea) - 1] == '\n')
            linea[strlen(linea) - 1] = '\0';

        if (strlen(linea) == 0) continue;

        int r = sscanf(linea, "%99[^|]|%99[^|]|%14[^|]|%9[^|]|%d",
                       nombre, productora, fecha, hora, &idSitio);

        if (r != 5) {
            printf("Linea inválida (evento): %s\n", linea);
            continue;
        }

        struct Evento *temp = realloc(eventos, (numEventos + 1) * sizeof(struct Evento));
        if (!temp) {
            printf("Error de memoria eventos\n");
            fclose(f);
            return;
        }

        eventos = temp;
        strcpy(eventos[numEventos].nombre, nombre);
        strcpy(eventos[numEventos].productora, productora);
        strcpy(eventos[numEventos].fecha, fecha);
        strcpy(eventos[numEventos].hora, hora);
        eventos[numEventos].idSitio = idSitio;
        eventos[numEventos].preciosSectores = NULL;
        numEventos++;
    }

    fclose(f);
}

/*
 * loadEvents
 *
 * Objetivo: Invocar la carga de eventos desde el archivo definido en RUTA_EVENTOS
 *
 * Entradas: Ninguna
 *
 * Salidas: void
 *
 * Restricciones: Se espera que el archivo exista en la ruta definida por RUTA_EVENTOS
 */
void loadEvents() {
    cargarEventosDesdeArchivo(RUTA_EVENTOS);
}

/*
 * savePrices
 *
 * Objetivo: Escribir los precios por sector de cada evento en el archivo de precios
 *
 * Entradas: Ninguna
 *
 * Salidas: void
 *
 * Restricciones: Sin restricciones específicas
 */
void savePrices() {
    FILE *f = fopen(RUTA_PRECIOS, "w");
    if (!f) return;

    for (int i = 0; i < numEventos; i++) {
        if (eventos[i].preciosSectores == NULL) continue;
        int nSect = sitios[eventos[i].idSitio].numSectores;

        //Escribe los precios
        for (int j = 0; j < nSect; j++) {
            fprintf(f, "%s|%d|%.2f\n", eventos[i].nombre, j,
                    eventos[i].preciosSectores[j]);
        }
    }
    fclose(f);
}

/*
 * loadPrices
 *
 * Objetivo: Leer el archivo de precios y asignarlos a los sectores
 * de cada evento correspondiente
 *
 * Entradas: Ninguna
 *
 * Salidas: void
 *
 * Restricciones: Los eventos deben estar cargados previamente en el sistema
 * antes de invocar esta función
 */
void loadPrices() {
    FILE *f = fopen(RUTA_PRECIOS, "r");
    if (!f) return;

    char linea[MAX_LINEA], nombreEvento[MAX_NOMBRE_EVENTO];
    int secIdx;
    float precio;

    while (fgets(linea, sizeof(linea), f)) {
        linea[strcspn(linea, "\n")] = 0;
        if (strlen(linea) == 0) continue;

        if (sscanf(linea, "%99[^|]|%d|%f", nombreEvento, &secIdx, &precio) != 3)
            continue;

        //LLena los precios en el arreglo
        for (int i = 0; i < numEventos; i++) {
            if (strcmp(eventos[i].nombre, nombreEvento) == 0) {
                int nSect = sitios[eventos[i].idSitio].numSectores;

                if (eventos[i].preciosSectores == NULL) {
                    eventos[i].preciosSectores = calloc(nSect, sizeof(float));
                }

                if (secIdx >= 0 && secIdx < nSect)
                    eventos[i].preciosSectores[secIdx] = precio;
                break;
            }
        }
    }
    fclose(f);
}

/*
 * cargarSectoresDesdeArchivo
 *
 * Objetivo: Abrir el archivo de sectores y cargar su contenido al sitio
 * correspondiente dentro del arreglo global
 *
 * Entradas: const char *rutaArchivo - Ruta del archivo a leer
 *
 * Salidas: void
 *
 * Restricciones: Se espera que el archivo exista y tenga el formato
 * nombreSitio|nombreSector|codigo|capacidad|precio. Los sitios referenciados
 * deben estar cargados previamente en el sistema
 */
static void cargarSectoresDesdeArchivo(const char *rutaArchivo) {
    FILE *f = fopen(rutaArchivo, "r");
    if (!f) {
        printf("No se pudo abrir sectores.\n");
        return;
    }

    char linea[MAX_LINEA];
    char nombreSitio[MAX_NOMBRE_SITIO], nombreSector[MAX_NOMBRE_SECTOR];
    char codigo;
    int capacidad;
    float precio;

    while (fgets(linea, sizeof(linea), f)) {
        if (linea[strlen(linea) - 1] == '\n')
            linea[strlen(linea) - 1] = '\0';

        if (strlen(linea) == 0) continue;

        int r = sscanf(linea, "%49[^|]|%29[^|]|%c|%d|%f",
                       nombreSitio, nombreSector, &codigo, &capacidad, &precio);

        if (r != 5) {
            printf("Linea inválida (sector): %s\n", linea);
            continue;
        }

        int idx = findSiteIndexByName(nombreSitio);
        if (idx == -1) {
            printf("Sitio no encontrado para sector: %s\n", nombreSitio);
            continue;
        }

        struct Site *s = &sitios[idx];
        struct Sector *temp = realloc(s->sectores,
                        (s->numSectores + 1) * sizeof(struct Sector));

        if (!temp) {
            printf("Error de memoria sectores\n");
            fclose(f);
            return;
        }

        //Llena el arreglo
        s->sectores = temp;
        int n = s->numSectores;

        strcpy(s->sectores[n].nombre, nombreSector);
        s->sectores[n].codigoInicial = codigo;
        s->sectores[n].capacidad = capacidad;
        s->sectores[n].precioBase = precio;

        generarAsientos(&s->sectores[n]);
        s->numSectores++;
    }

    fclose(f);
}

/*
 * loadSegments
 *
 * Objetivo: Invocar la carga de sectores desde el archivo definido en RUTA_SECTORES
 *
 * Entradas: Ninguna
 *
 * Salidas: void
 *
 * Restricciones: Se espera que el archivo exista en la ruta definida por RUTA_SECTORES
 */
void loadSegments() {
    cargarSectoresDesdeArchivo(RUTA_SECTORES);
}

/*
 * guardarSectores
 *
 * Objetivo: Escribir la información de todos los sectores de cada sitio
 * en el archivo de sectores
 *
 * Entradas: Ninguna
 *
 * Salidas: void
 *
 * Restricciones: Sin restricciones específicas
 */
void guardarSectores() {
    FILE *f = fopen(RUTA_SECTORES, "w");
    if (!f) return;

    for (int i = 0; i < numSitios; i++) {
        for (int j = 0; j < sitios[i].numSectores; j++) {
            fprintf(f, "%s|%s|%c|%d|%.2f\n",
                    sitios[i].nombre,
                    sitios[i].sectores[j].nombre,
                    sitios[i].sectores[j].codigoInicial,
                    sitios[i].sectores[j].capacidad,
                    sitios[i].sectores[j].precioBase);
        }
    }
    fclose(f);
}

/*
 * guardarEventos
 *
 * Objetivo: Escribir la información de todos los eventos en el archivo de eventos
 *
 * Entradas: Ninguna
 *
 * Salidas: void
 *
 * Restricciones: Sin restricciones específicas
 */
void guardarEventos() {
    FILE *f = fopen(RUTA_EVENTOS, "w");
    if (!f) return;
    for (int i = 0; i < numEventos; i++) {
        fprintf(f, "%s|%s|%s|%s|%d\n",
            eventos[i].nombre,
            eventos[i].productora,
            eventos[i].fecha,
            eventos[i].hora,
            eventos[i].idSitio);
    }
    fclose(f);
}