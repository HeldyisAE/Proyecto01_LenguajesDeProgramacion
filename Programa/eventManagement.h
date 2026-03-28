#ifndef EVENTMANAGEMENT_H
#define EVENTMANAGEMENT_H

#include "siteManagement.h"

//Define macros de direcciones
#define RUTA_EVENTOS    "datos/eventos.txt"
#define RUTA_SECTORES   "datos/sectores.txt"
#define RUTA_PRECIOS    "datos/precios.txt"

//Define macros de constantes
#define MAX_NOMBRE_EVENTO       100
#define MAX_PRODUCTORA          100
#define MAX_NOMBRE_SECTOR       30
#define MAX_NOMBRE_SITIO        50
#define MAX_FECHA_EVENTO        15
#define MAX_HORA_EVENTO         10

//Define estructuras
struct Sector {
    char nombre[30];
    char codigoInicial;
    int capacidad;
    float precioBase;
    char **asientos;
};

struct Evento {
    char nombre[100];
    char fecha[15];
    char hora[10];
    int idSitio;            
    float *preciosSectores;
    char productora[100];
};

//Define variables globales
extern struct Evento *eventos;
extern int numEventos;

//Define prototipos
void gestionarSectores();
void gestionarEventos();
void guardarSectores();
void guardarEventos();
void loadEvents();
void loadSegments();
void savePrices();
void mostrarSitiosYSectores();
void agregarSectorASitio();
void resetearSectores();
void generarAsientos(struct Sector *sec);
void liberarAsientos(struct Sector *sec);

#endif