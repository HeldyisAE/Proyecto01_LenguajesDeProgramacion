#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eventManagement.h"

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
    
    printf("Nombre del Evento: ");
    fgets(eventos[numEventos].nombre, 100, stdin);
    eventos[numEventos].nombre[strcspn(eventos[numEventos].nombre, "\n")] = 0;

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
    scanf("%d", &sel);
    limpiarBuffer();
    
    int sIdx = sel - 1;
    eventos[numEventos].idSitio = sIdx;

    int nSect = sitios[sIdx].numSectores;
    if (nSect > 0) {
        eventos[numEventos].preciosSectores = (float *)malloc(nSect * sizeof(float));
        printf("\n--- Ajuste de Precios para este Evento ---\n");
        for (int i = 0; i < nSect; i++) {
            printf("  Precio para %s (Base: %.2f): ", 
                   sitios[sIdx].sectores[i].nombre, 
                   sitios[sIdx].sectores[i].precioBase);
            scanf("%f", &eventos[numEventos].preciosSectores[i]);
        }
        limpiarBuffer();
    } else {
        eventos[numEventos].preciosSectores = NULL;
    }

    numEventos++;
    printf("\nEvento registrado.\n");
    guardarEventos();
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
        fprintf(f, "%s|%s|%s|%d\n", 
                eventos[i].nombre, eventos[i].fecha, 
                eventos[i].hora, eventos[i].idSitio);
    }
    fclose(f);
}