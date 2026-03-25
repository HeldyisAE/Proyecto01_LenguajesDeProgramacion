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
    printf("4. Mostrar sitios\n");
    printf("0. Volver\n");
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

int findSiteIndexByName(const char *nombre) {
    for (int i = 0; i < numSitios; i++) {
        if (strcmp(sitios[i].nombre, nombre) == 0) {
            return i;
        }
    }
    return -1;
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
        sitios[numSitios].sectores = NULL;
        sitios[numSitios].numSectores = 0;
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
    
    printf("Ingrese la dirección del archivo que desea cargar: ");
    fgets(rutaArchivo, sizeof(rutaArchivo), stdin);
    
    // Elimina salto de línea si existe
    if (rutaArchivo[strlen(rutaArchivo) - 1] == '\n') {
        rutaArchivo[strlen(rutaArchivo) - 1] = '\0';
    }
    
    cargarSitiosDesdeArchivo(rutaArchivo, 0);
    
    saveFile();
}

void editSite() {
    if (numSitios == 0) {
        printf("\nNo hay sitios disponibles para editar.\n");
        return;
    }
    
    char siteName[50];
    int siteIndex;
    int optEdit;
    char newValue[50];
    
    printf("\nIngrese el nombre del sitio a modificar: ");
    fgets(siteName, sizeof(siteName), stdin);

    if (siteName[strlen(siteName) - 1] == '\n') {
        siteName[strlen(siteName) - 1] = '\0';
    }
    
    trimWhitespace(siteName);
    
    siteIndex = findSiteIndexByName(siteName);
    
    if (siteIndex == -1) {
        printf("\nError: No se encontró el sitio '%s'.\n", siteName);
        return;
    }

    printf("\n--- Información actual del sitio ---\n");
    printf("Nombre: %s\n", sitios[siteIndex].nombre);
    printf("Ubicación: %s\n", sitios[siteIndex].ubicacion);
    printf("Sitio web: %s\n", sitios[siteIndex].web);
    
    printf("\n--- Seleccione qué desea editar ---\n");
    printf("1. Editar nombre\n");
    printf("2. Editar ubicación\n");
    printf("3. Editar sitio web\n");
    printf("4. Volver\n");
    printf("> ");
    scanf("%d", &optEdit);
    limpiarBuffer();
    
    switch (optEdit) {
        case 1:
            printf("Ingrese el nuevo nombre: ");
            fgets(newValue, sizeof(newValue), stdin);
            if (newValue[strlen(newValue) - 1] == '\n') {
                newValue[strlen(newValue) - 1] = '\0';
            }
            trimWhitespace(newValue);
            
            if (strlen(newValue) == 0) {
                printf("Error: El nombre no puede estar vacío.\n");
            } else {
                strcpy(sitios[siteIndex].nombre, newValue);
                printf("Nombre actualizado correctamente.\n");
            }
            break;
            
        case 2:
            printf("Ingrese la nueva ubicación: ");
            fgets(newValue, sizeof(newValue), stdin);
            if (newValue[strlen(newValue) - 1] == '\n') {
                newValue[strlen(newValue) - 1] = '\0';
            }
            trimWhitespace(newValue);
            
            if (strlen(newValue) == 0) {
                printf("Error: La ubicación no puede estar vacía.\n");
            } else {
                strcpy(sitios[siteIndex].ubicacion, newValue);
                printf("Ubicación actualizada correctamente.\n");
            }
            break;
            
        case 3:
            printf("Ingrese el nuevo sitio web: ");
            fgets(newValue, sizeof(newValue), stdin);
            if (newValue[strlen(newValue) - 1] == '\n') {
                newValue[strlen(newValue) - 1] = '\0';
            }
            trimWhitespace(newValue);
            strcpy(sitios[siteIndex].web, newValue);
            printf("Sitio web actualizado correctamente.\n");
            break;
            
        case 4:
            printf("Operación cancelada.\n");
            return;
            
        default:
            printf("Opción no válida.\n");
            return;
    }
    
    saveFile();
}

void deleteSite() {
    if (numSitios == 0) {
        printf("\nNo hay sitios disponibles para eliminar.\n");
        return;
    }
    
    char siteName[50];
    int siteIndex;
    int confirm;
    
    printf("\nIngrese el nombre del sitio a eliminar: ");
    fgets(siteName, sizeof(siteName), stdin);

    if (siteName[strlen(siteName) - 1] == '\n') {
        siteName[strlen(siteName) - 1] = '\0';
    }
    
    trimWhitespace(siteName);
    
    siteIndex = findSiteIndexByName(siteName);
    
    if (siteIndex == -1) {
        printf("\nError: No se encontró el sitio '%s'.\n", siteName);
        return;
    }

    printf("\n--- Información del sitio a eliminar ---\n");
    printf("Nombre: %s\n", sitios[siteIndex].nombre);
    printf("Ubicación: %s\n", sitios[siteIndex].ubicacion);
    printf("Sitio web: %s\n", sitios[siteIndex].web);
    
    printf("\n--- Confirmar eliminación ---\n");
    printf("¿Desea eliminar este sitio?\n");
    printf("1. Confirmar eliminación\n");
    printf("2. Cancelar\n");
    printf("> ");
    scanf("%d", &confirm);
    limpiarBuffer();
    
    if (confirm == 1) {
        // Desplaza los elementos posteriores una posición hacia atrás
        for (int i = siteIndex; i < numSitios - 1; i++) {
            sitios[i] = sitios[i + 1];
        }
        numSitios--;
        
        printf("\nSitio eliminado correctamente.\n");

        saveFile();
    } else if (confirm == 2) {
        printf("\nOperación cancelada.\n");
    } else {
        printf("\nOpción no válida.\n");
    }
}

void showSites() {
    if (numSitios == 0) {
        printf("\nNo hay sitios registrados.\n");
        return;
    }

    printf("\n========================================\n");
    printf("         SITIOS REGISTRADOS (%d)\n", numSitios);
    printf("========================================\n");

    for (int i = 0; i < numSitios; i++) {
        printf("\n[%d] %s\n", i + 1, sitios[i].nombre);
        printf("    Ubicación : %s\n", sitios[i].ubicacion);

        if (strlen(sitios[i].web) > 0) {
            printf("    Sitio web : %s\n", sitios[i].web);
        } else {
            printf("    Sitio web : (no registrado)\n");
        }

        if (i < numSitios - 1) {
            printf("    ----------------------------------------\n");
        }
    }

    printf("\n========================================\n");
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
                printf("\n--- Mostrando sitios ---\n");
                showSites();
                break;
            case 0:
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