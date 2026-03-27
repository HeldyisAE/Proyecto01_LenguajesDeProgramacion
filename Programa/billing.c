#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "billing.h"

// =========================================================
// Ventas echas
// =========================================================
void mostrarListaFacturas() {
    FILE *f = fopen("datos/facturas.txt", "r");
    if (!f) {
        printf("\n[!] No hay facturas registradas.\n");
        return;
    }

    printf("\n==========================================================\n");
    printf("                HISTORIAL DE FACTURAS\n");
    printf("==========================================================\n");
    printf("%-5s | %-15s | %-15s | %-10s\n", "ID", "Cliente", "Evento", "Monto");
    printf("----------------------------------------------------------\n");

    int id, asiento;
    char cliente[50], evento[100], sector[30];
    float monto;

    while (fscanf(f, "%d|%[^|]|%[^|]|%[^|]|%d|%f\n", &id, cliente, evento, sector, &asiento, &monto) != EOF) {
        printf("%-5d | %-15.15s | %-15.15s | %-10.2f\n", id, cliente, evento, monto);
    }

    fclose(f);
    printf("==========================================================\n");
}

// =========================================================
void mostrarEstadisticas() {
    FILE *f = fopen("datos/facturas.txt", "r");
    if (!f) {
        printf("\n[!] No hay datos suficientes para generar estadisticas.\n");
        return;
    }

    float recaudacionTotal = 0;
    int totalBoletos = 0;
    
    int id, asiento;
    char cliente[50], evento[100], sector[30];
    float monto;

    // esto es para encontrar el evento mas vendido
    char eventoMasVendido[100] = "N/A";
    int maxVentas = 0;

    while (fscanf(f, "%d|%[^|]|%[^|]|%[^|]|%d|%f\n", &id, cliente, evento, sector, &asiento, &monto) != EOF) {
        recaudacionTotal += monto;
        totalBoletos++;
    }
    fclose(f);

    printf("\n==========================================\n");
    printf("         REPORTE DE ESTADISTICAS\n");
    printf("==========================================\n");
    printf("  Total de boletos vendidos : %d\n", totalBoletos);
    printf("  Recaudacion Total Bruta   : %.2f CRC\n", recaudacionTotal);
    if (totalBoletos > 0) {
        printf("  Promedio por factura      : %.2f CRC\n", recaudacionTotal / totalBoletos);
    }
    printf("==========================================\n");
}