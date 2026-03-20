#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "siteManagement.h"

struct Site *sitios = NULL;
int numSitios = 0;
static int capacidad = 0;
static const int INCREMENTO_MEMORIA = 5;

static void limpiarBuffer();
static int siteExists(const char *nombre);
static void trimWhitespace(char *str);
static void cargarSitiosDesdeArchivo(const char *rutaArchivo, int esInicial);

int menuSitesManagement() {
    int option;
    printf("\n-------Gestión de Sitios de Eventos-------\n");
    printf("1. Cargar archivo de sitios\n");
    printf("2. Editar sitios\n");
    printf("3. Eliminar sitio\n");
    printf("4. Volver al menú anterior\n");
    printf("> ");
    scanf("%d", &option);    
    limpiarBuffer();  // Limpia el salto de línea después de scanf
    return option;
}

static int siteExists(const char *nombre) {
    for (int i = 0; i < numSitios; i++) {
        if (strcmp(sitios[i].nombre, nombre) == 0) {
            return 1;
        }
    }
    return 0;
}

static void trimWhitespace(char *str) {
    int start = 0, end = strlen(str) - 1;
    
    // Elimina espacios al inicio
    while (start <= end && (str[start] == ' ' || str[start] == '\t')) {
        start++;
    }
    
    // Elimina espacios al final
    while (end >= start && (str[end] == ' ' || str[end] == '\t')) {
        end--;
    }
    
    // Copia cadena limpia
    int j = 0;
    for (int i = start; i <= end; i++) {
        str[j++] = str[i];
    }
    str[j] = '\0';
}

// Función auxiliar para limpiar el buffer de entrada
static void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static void cargarSitiosDesdeArchivo(const char *rutaArchivo, int esInicial) {
    FILE *archivo;
    char linea[200];
    char nombre[50], ubicacion[50], web[50];
    int resultado;
    int sitiosAgregados = 0;
    int sitiosDuplicados = 0;
    
    archivo = fopen(rutaArchivo, "r");
    if (archivo == NULL) {
        if (esInicial) {
            printf("Nota: No se encontró archivo de sitios. Se inicializa vacío.\n");
        } else {
            printf("Error: No se pudo abrir el archivo '%s'\n", rutaArchivo);
        }
        return;
    }
    
    while (fgets(linea, sizeof(linea), archivo) != NULL) {

        if (linea[strlen(linea) - 1] == '\n') {
            linea[strlen(linea) - 1] = '\0';
        }
        
        //Lineas en blanco
        if (strlen(linea) == 0) continue;
        
        // Parsear la línea
        resultado = sscanf(linea, "%49[^,], %49[^,], %49[^\n]", nombre, ubicacion, web);
        
        // Si no hay web sscanf retorna 2
        if (resultado < 2) {
            if (!esInicial) {
                printf("Advertencia: Línea malformada (se ignora): %s\n", linea);
            }
            continue;
        }
        

        trimWhitespace(nombre);
        trimWhitespace(ubicacion);
        if (resultado == 3) {
            trimWhitespace(web);
        } else {
            web[0] = '\0';
        }
        
        // Verifica unicidad
        if (siteExists(nombre)) {
            if (!esInicial) {
                printf("Advertencia: El sitio '%s' ya existe. Se ignora.\n", nombre);
            }
            sitiosDuplicados++;
            continue;
        }
        
        // Expande memoria si es necesario
        if (numSitios >= capacidad) {
            capacidad += INCREMENTO_MEMORIA;
            struct Site *temp = realloc(sitios, capacidad * sizeof(struct Site));
            if (temp == NULL) {
                printf("Error: No se pudo asignar memoria\n");
                fclose(archivo);
                return;
            }
            sitios = temp;
        }
        
        // Agrega sitio al arreglo
        strcpy(sitios[numSitios].nombre, nombre);
        strcpy(sitios[numSitios].ubicacion, ubicacion);
        strcpy(sitios[numSitios].web, web);
        numSitios++;
        sitiosAgregados++;
    }
    
    fclose(archivo);
    
    if (!esInicial) {
        printf("\n--- Resumen de carga ---\n");
        printf("Sitios agregados: %d\n", sitiosAgregados);
        printf("Sitios duplicados (ignorados): %d\n", sitiosDuplicados);
        printf("Total de sitios en el sistema: %d\n", numSitios);
    }
}

void saveFile() {
    FILE *archivo;
    
    archivo = fopen("datos/sitios.txt", "w");
    if (archivo == NULL) {
        printf("Error: No se pudo guardar el archivo de sitios\n");
        return;
    }
    
    // Escribe cada sitio en el formato: nombre,ubicacion,web
    for (int i = 0; i < numSitios; i++) {
        fprintf(archivo, "%s,%s,%s\n", sitios[i].nombre, sitios[i].ubicacion, sitios[i].web);
    }
    
    fclose(archivo);
}

void loadInitialSites() {
    cargarSitiosDesdeArchivo("datos/sitios.txt", 1);
}

void loadFile() {
    char rutaArchivo[256];
    
    printf("Ingrese la dirección del archivo que desea cargar:\n");
    limpiarBuffer();  // Limpia el buffer de entrada
    fgets(rutaArchivo, sizeof(rutaArchivo), stdin);
    
    // Elimina salto de línea si existe
    if (rutaArchivo[strlen(rutaArchivo) - 1] == '\n') {
        rutaArchivo[strlen(rutaArchivo) - 1] = '\0';
    }
    
    cargarSitiosDesdeArchivo(rutaArchivo, 0);
    
    saveFile();
}

void editSite() {
    // Función a implementar
}

void deleteSite() {
    // Función a implementar
}

void manageSitesMenu() {
    int option;
    
    for (;;) {
        option = menuSitesManagement();
        
        switch (option) {
            case 1:
                printf("\n--- Cargando archivo de sitios ---\n");
                loadFile();
                break;
            case 2:
                printf("\n--- Editando sitio ---\n");
                editSite();
                break;
            case 3:
                printf("\n--- Eliminando sitio ---\n");
                deleteSite();
                break;
            case 4:
                printf("Volviendo al menú anterior...\n");
                return; 
            default:
                printf("Opción no válida. Intente de nuevo.\n");
                break;
        }
        
        printf("\nPresione Enter para continuar...");
        getchar();  // Espera a que el usuario presione Enter
    }
}