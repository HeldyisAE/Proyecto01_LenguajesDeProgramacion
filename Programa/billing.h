#ifndef BILLING_H
#define BILLING_H

// representa factura
struct Factura {
    int idFactura;
    char cliente[50];
    char eventoNombre[100];
    char sectorNombre[30];
    int asiento;
    float montoTotal;
};

void mostrarListaFacturas();
void mostrarEstadisticas();

#endif