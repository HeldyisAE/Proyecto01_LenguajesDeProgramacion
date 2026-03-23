#ifndef EVENTMANAGEMENT_H
#define EVENTMANAGEMENT_H

#include "siteManagement.h"

struct Sector {
    char nombre[30];
    char codigoInicial;
    int capacidad;
    float precioBase;
};

struct Evento {
    char nombre[100];
    char fecha[15];
    char hora[10];
    int idSitio;            
    float *preciosSectores;
};

extern struct Evento *eventos;
extern int numEventos;

void gestionarSectores();
void gestionarEventos();
void guardarSectores();
void guardarEventos();

#endif