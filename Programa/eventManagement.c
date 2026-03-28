#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eventManagement.h"
#include "siteManagement.h"

struct Evento *eventos = NULL;
int numEventos = 0;

static void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void generarAsientos(struct Sector *sec) {
    sec->asientos = (char **)malloc(sec->capacidad * sizeof(char*));

    for (int i = 0; i < sec->capacidad; i++) {
        sec->asientos[i] = (char *)malloc(10 * sizeof(char));
        sprintf(sec->asientos[i], "%c%d", sec->codigoInicial, i + 1);
    }
}

void liberarAsientos(struct Sector *sec) {
    for (int i = 0; i < sec->capacidad; i++) {
        free(sec->asientos[i]);
    }
    free(sec->asientos);
}

// =========================================================
// GESTIÓN DE ESPACIOS (SECTORES)
// =========================================================
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

    struct Sector *temp = realloc(s->sectores,
        (s->numSectores + 1) * sizeof(struct Sector));

    if (!temp) return;

    s->sectores = temp;

    int sectoresToAdd;

    printf("\n¿Cuántos sectores desea agregar?: ");
    scanf("%d", &sectoresToAdd);
    limpiarBuffer();

    for(int i = 0; i < sectoresToAdd; i++) {

        struct Sector *nuevo = &s->sectores[s->numSectores];

        printf("\n--- Sector %d ---\n", i + 1);

        printf("\nNombre sector: ");
        fgets(nuevo->nombre, 30, stdin);
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

    free(s->sectores);
    s->sectores = NULL;
    s->numSectores = 0;

    guardarSectores();

    printf("Sectores eliminados.\n");
}

// =========================================================
// GESTIÓN DE EVENTOS
// =========================================================
void gestionarEventos() {
    if (numSitios == 0) {
        printf("\n ! No se pueden crear eventos sin sitios.\n");
        return;
    }

    printf("\n--- REGISTRAR NUEVO EVENTO ---\n");
    
    struct Evento *temp = (struct Evento *)realloc(eventos, (numEventos + 1) * sizeof(struct Evento));
    if (temp == NULL) {
        printf(" ! Error de memoria.\n");
        return;
    }
    eventos = temp;

    limpiarBuffer();
    
    printf("Nombre del Evento: ");
    fgets(eventos[numEventos].nombre, 100, stdin);
    eventos[numEventos].nombre[strcspn(eventos[numEventos].nombre, "\n")] = 0;
    
    printf("Productora: ");
    fgets(eventos[numEventos].productora, 100, stdin);
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

    // Valida que el sitio tenga sectores
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


static void cargarEventosDesdeArchivo(const char *rutaArchivo) {
    FILE *f = fopen(rutaArchivo, "r");
    if (!f) {
        printf("No se pudo abrir eventos.\n");
        return;
    }

    char linea[256];
    char nombre[100], productora[100], fecha[15], hora[10];
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

void loadEvents() {
    cargarEventosDesdeArchivo("datos/eventos.txt");
}

void savePrices() {
    FILE *f = fopen("datos/precios.txt", "w");
    if (!f) return;
    for (int i = 0; i < numEventos; i++) {
        if (eventos[i].preciosSectores == NULL) continue;
        int nSect = sitios[eventos[i].idSitio].numSectores;
        for (int j = 0; j < nSect; j++) {
            fprintf(f, "%s|%d|%.2f\n", eventos[i].nombre, j, 
                    eventos[i].preciosSectores[j]);
        }
    }
    fclose(f);
}

void loadPrices() {
    FILE *f = fopen("datos/precios.txt", "r");
    if (!f) return;

    char linea[256], nombreEvento[100];
    int secIdx;
    float precio;

    while (fgets(linea, sizeof(linea), f)) {
        linea[strcspn(linea, "\n")] = 0;
        if (strlen(linea) == 0) continue;

        if (sscanf(linea, "%99[^|]|%d|%f", nombreEvento, &secIdx, &precio) != 3)
            continue;

        // Buscar el evento por nombre
        for (int i = 0; i < numEventos; i++) {
            if (strcmp(eventos[i].nombre, nombreEvento) == 0) {
                int nSect = sitios[eventos[i].idSitio].numSectores;

                // Alojar si aún no se ha hecho
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

static void cargarSectoresDesdeArchivo(const char *rutaArchivo) {
    FILE *f = fopen(rutaArchivo, "r");
    if (!f) {
        printf("No se pudo abrir sectores.\n");
        return;
    }

    char linea[256];
    char nombreSitio[50], nombreSector[30];
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

void loadSegments() {
    cargarSectoresDesdeArchivo("datos/sectores.txt");
}


// =========================================================
// Guarda la info de los sectores y los eventos en el txt
// =========================================================
void guardarSectores() {
    FILE *f = fopen("datos/sectores.txt", "w");
    if (!f) return;
    for (int i = 0; i < numSitios; i++) {
        for (int j = 0; j < sitios[i].numSectores; j++) {
            fprintf(f, "%s|%s|%c|%d|%.2f\n", 
                    sitios[i].nombre, sitios[i].sectores[j].nombre,
                    sitios[i].sectores[j].codigoInicial,
                    sitios[i].sectores[j].capacidad,
                    sitios[i].sectores[j].precioBase);
        }
    }
    fclose(f);
}

void guardarEventos() {
    FILE *f = fopen("datos/eventos.txt", "w");
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