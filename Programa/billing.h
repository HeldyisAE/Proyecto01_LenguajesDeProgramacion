#ifndef BILLING_H
#define BILLING_H

struct Factura {
    int idFactura;
    char cliente[50];
    char eventoNombre[100];
    char sectorNombre[30];
    char fechaCompra[15];
    int asiento;
    float montoTotal;
};

struct MesStats {
    char mesAnio[10];
    int cantidad;
};

struct ProductoraStats {
    char nombre[100];
    float recaudacion;
};

struct SitioStats {
    char nombre[100];
    int cantidadEventos;
    float recaudacion;
};

void mostrarListaFacturas();
void mostrarEstadisticas();

#endif