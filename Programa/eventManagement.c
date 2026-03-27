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

// =========================================================
// GESTIÓN DE ESPACIOS (SECTORES)
// =========================================================
void gestionarSectores() {
    if (numSitios == 0) {
        printf("\n ! No hay sitios registrados. Primero debe crear un sitio.\n");
        return;
    }

    printf("\n--- CONFIGURACION DE ESPACIOS y SECTORES ---\n");
    for (int i = 0; i < numSitios; i++) {
        printf("%d. %s\n", i + 1, sitios[i].nombre);
    }
    
    int sel;
    printf("Seleccione el lugar: ");
    if (scanf("%d", &sel) != 1 || sel < 1 || sel > numSitios) {
        printf(" ! Seleccion invalida.\n");
        limpiarBuffer();
        return;
    }
    limpiarBuffer();

    int idx = sel - 1;
    int cant;
    printf("¿Cuantos sectores desea agregar a '%s'?: ", sitios[idx].nombre);
    scanf("%d", &cant);
    limpiarBuffer();

    struct Sector *temp = (struct Sector *)realloc(sitios[idx].sectores, 
                            (sitios[idx].numSectores + cant) * sizeof(struct Sector));

    if (temp == NULL) {
        printf(" ! Error de memoria.\n");
        return;
    }
    sitios[idx].sectores = temp;

    for (int i = 0; i < cant; i++) {
        int n = sitios[idx].numSectores;
        printf("\nConfigurando Sector #%d:\n", n + 1);
        
        printf("  Nombre (VIP, Sol, etc): ");
        fgets(sitios[idx].sectores[n].nombre, 30, stdin);
        sitios[idx].sectores[n].nombre[strcspn(sitios[idx].sectores[n].nombre, "\n")] = 0;

        printf("  Letra identificadora (A, B...): ");
        scanf(" %c", &sitios[idx].sectores[n].codigoInicial);

        printf("  Capacidad: ");
        scanf("%d", &sitios[idx].sectores[n].capacidad);

        printf("  Precio base: ");
        scanf("%f", &sitios[idx].sectores[n].precioBase);
        limpiarBuffer();

        sitios[idx].numSectores++;
    }

    printf("\nSectores configurados.\n");
    guardarSectores();
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

        eventos[numEventos].preciosSectores = NULL; // Importante

        numEventos++;
    }

    fclose(f);
    printf("Eventos cargados: %d\n", numEventos);
}

void loadEvents() {
    cargarEventosDesdeArchivo("datos/eventos.txt");
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

        s->numSectores++;
    }

    fclose(f);
    printf("Sectores cargados correctamente.\n");
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