#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "billing.h"
#include "eventManagement.h"

/*
 * topMeses
 *
 * Objetivo: Calcular y mostrar los meses con mayor cantidad de eventos registrados
 *
 * Entradas: Ninguna
 *
 * Salidas: void
 *
 * Restricciones: El arreglo global de eventos debe estar inicializado.
 * Las fechas de los eventos deben tener el formato DD/MM/AAAA
 */
void topMeses() {
    struct MesStats meses[100];
    int count = 0;

    for (int i = 0; i < numEventos; i++) {
        char mesAnio[10];

        //Extrae el mes y el año
        strncpy(mesAnio, eventos[i].fecha + 3, 7);
        mesAnio[7] = '\0';

        int encontrado = -1;

        for (int j = 0; j < count; j++) {
            if (strcmp(meses[j].mesAnio, mesAnio) == 0) {
                encontrado = j;
                break;
            }
        }

        if (encontrado != -1) {
            meses[encontrado].cantidad++;
        } else if (count < 100) {
            strcpy(meses[count].mesAnio, mesAnio);
            meses[count].cantidad = 1;
            count++;
        }
    }

    // Ordena de mayor a menor
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (meses[j].cantidad < meses[j + 1].cantidad) {
                struct MesStats temp = meses[j];
                meses[j] = meses[j + 1];
                meses[j + 1] = temp;
            }
        }
    }

    printf("\nTop 3 meses con mas eventos:\n");
    for (int i = 0; i < 3 && i < count; i++) {
        printf("%s -> %d eventos\n", meses[i].mesAnio, meses[i].cantidad);
    }
}

/*
 * topProductoras
 *
 * Objetivo: Calcular y mostrar las productoras con mayor recaudación
 * a partir del historial de facturas
 *
 * Entradas: Ninguna
 *
 * Salidas: void
 *
 * Restricciones: El arreglo global de eventos debe estar inicializado.
 * Se espera que el archivo de facturas exista en RUTA_FACTURAS
 */
void topProductoras() {
    struct ProductoraStats prod[100];
    int count = 0;

    FILE *f = fopen("datos/facturas.txt", "r");
    if (!f) return;

    int id, asiento;
    char cliente[50], eventoNombre[100], sector[30], fecha[15];
    float monto;

    while (fscanf(f, "%d|%[^|]|%[^|]|%[^|]|%d|%f|%[^|\n]", &id, cliente, eventoNombre, sector, &asiento, &monto, fecha) == 7) {

        for (int i = 0; i < numEventos; i++) {
            if (strcmp(eventos[i].nombre, eventoNombre) == 0) {

                char *prodNombre = eventos[i].productora;
                int encontrado = -1;

                for (int j = 0; j < count; j++) {
                    if (strcmp(prod[j].nombre, prodNombre) == 0) {
                        encontrado = j;
                        break;
                    }
                }

                if (encontrado != -1) {
                    prod[encontrado].recaudacion += monto;
                } else if (count < 100) {
                    strcpy(prod[count].nombre, prodNombre);
                    prod[count].recaudacion = monto;
                    count++;
                }

                break;
            }
        }
    }

    fclose(f);

    // Ordena de mayor a menor
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (prod[j].recaudacion < prod[j + 1].recaudacion) {
                struct ProductoraStats temp = prod[j];
                prod[j] = prod[j + 1];
                prod[j + 1] = temp;
            }
        }
    }

    printf("\nTop 3 productoras:\n");
    for (int i = 0; i < 3 && i < count; i++) {
        printf("%s -> %.2f CRC\n", prod[i].nombre, prod[i].recaudacion);
    }
}

/*
 * estadisticasSitios
 *
 * Objetivo: Calcular y mostrar la cantidad de eventos y recaudación total
 * por sitio, ordenados de mayor a menor recaudación
 *
 * Entradas: Ninguna
 *
 * Salidas: void
 *
 * Restricciones: Los arreglos globales de eventos y sitios deben estar inicializados.
 * Se espera que el archivo de facturas exista en RUTA_FACTURAS
 */
void estadisticasSitios() {
    struct SitioStats stats[100];

    //Carga estadísticas con los sitios actuales
    for (int i = 0; i < numSitios; i++) {
        strcpy(stats[i].nombre, sitios[i].nombre);
        stats[i].cantidadEventos = 0;
        stats[i].recaudacion = 0;
    }

    // Cuenta eventos por sitio
    for (int i = 0; i < numEventos; i++) {
        for (int j = 0; j < numSitios; j++) {
            if (eventos[i].idSitio == j) {
                stats[j].cantidadEventos++;
                break;
            }
        }
    }

    FILE *f = fopen("datos/facturas.txt", "r");
    if (!f) return;

    int id, asiento;
    char cliente[50], eventoNombre[100], sector[30], fecha[15];
    float monto;

    //Acomula la recaudación de cada sitio
    while (fscanf(f, "%d|%[^|]|%[^|]|%[^|]|%d|%f|%[^|\n]", &id, cliente, eventoNombre, sector, &asiento, &monto, fecha) == 7) {

        for (int i = 0; i < numEventos; i++) {
            if (strcmp(eventos[i].nombre, eventoNombre) == 0) {

                for (int j = 0; j < numSitios; j++) {
                    if (eventos[i].idSitio == j) {
                        stats[j].recaudacion += monto;
                        break;
                    }
                }

                break;
            }
        }
    }

    fclose(f);

    // Ordena por recaudación
    for (int i = 0; i < numSitios - 1; i++) {
        for (int j = 0; j < numSitios - i - 1; j++) {
            if (stats[j].recaudacion < stats[j + 1].recaudacion) {
                struct SitioStats temp = stats[j];
                stats[j] = stats[j + 1];
                stats[j + 1] = temp;
            }
        }
    }

    printf("\nSitios:\n");
    for (int i = 0; i < numSitios; i++) {
        printf("%s -> Eventos: %d | Recaudacion: %.2f\n",
               stats[i].nombre,
               stats[i].cantidadEventos,
               stats[i].recaudacion);
    }
}

/*
 * mostrarListaFacturas
 *
 * Objetivo: Leer el archivo de facturas y mostrar su contenido en formato tabular
 *
 * Entradas: Ninguna
 *
 * Salidas: void
 *
 * Restricciones: Se espera que el archivo de facturas exista en RUTA_FACTURAS
 */
void mostrarListaFacturas() {
    FILE *f = fopen("datos/facturas.txt", "r");
    if (!f) {
        printf("\n[!] No hay facturas registradas.\n");
        return;
    }

    int id, asiento;
    char cliente[50], evento[100], sector[30], fecha[15];
    float subtotal;

    printf("\n====================================================================\n");
    printf("                     HISTORIAL DE FACTURAS\n");
    printf("====================================================================\n");

    while (fscanf(f, "%d|%[^|]|%[^|]|%[^|]|%d|%f|%[^|\n]",
                  &id, cliente, evento, sector, &asiento, &subtotal, fecha) == 7) {

        printf("%-5d | %-20.20s | %-12s | %-15.15s | %-10.2f\n",
               id, evento, fecha, cliente, subtotal);
    }

    fclose(f);
}

/*
 * mostrarEstadisticas
 *
 * Objetivo: Calcular y mostrar el reporte general de facturación e invocar
 * las funciones de estadísticas detalladas
 *
 * Entradas: Ninguna
 *
 * Salidas: void
 *
 * Restricciones: Se espera que el archivo de facturas exista en RUTA_FACTURAS
 */
void mostrarEstadisticas() {
    FILE *f = fopen("datos/facturas.txt", "r");
    if (!f) {
        printf("\n[!] No hay datos suficientes.\n");
        return;
    }

    float total = 0;
    int cantidad = 0;

    int id, asiento;
    char cliente[50], evento[100], sector[30], fecha[15];
    float subtotal;

    while (fscanf(f, "%d|%[^|]|%[^|]|%[^|]|%d|%f|%[^|\n]",
                  &id, cliente, evento, sector, &asiento, &subtotal, fecha) == 7) {

        total += subtotal;
        cantidad++;
    }

    fclose(f);

    printf("\n==========================================\n");
    printf("               REPORTE\n");
    printf("==========================================\n");
    printf("Total de facturas: %d\n", cantidad);
    printf("Recaudacion total: %.2f CRC\n", total);

    if (cantidad > 0) {
        printf("Promedio: %.2f CRC\n", total / cantidad);
    }

    printf("==========================================\n");

    printf("\n===== DETALLE ESTADÍSTICO =====\n");
    topMeses();
    topProductoras();
    estadisticasSitios();
}