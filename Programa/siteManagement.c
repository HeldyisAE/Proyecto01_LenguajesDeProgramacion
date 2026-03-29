#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "siteManagement.h"

//Definición de variables globales
struct Site *sitios = NULL;
int numSitios = 0;
static int capacidad = 0; //Cantidad de sitios en sistema

//Prototipos de funciones estáticas
static void limpiarBuffer();
static int siteExists(const char *nombre);
static void trimWhitespace(char *str);
static void cargarSitiosDesdeArchivo(const char *rutaArchivo, int esInicial);

/*
 * menuSitesManagement
 *
 * Objetivo: Imprimir el menú de opciones para gestionar sitios de evento
 *
 * Entradas: option - Representa la elección del usuario dentro del menu
 *
 * Salidas: option - Retorna la opción para ser utilizada en main
 *
 * Restricciones: Se espera que el usuario ingrese un int dentro del
 * rango correspondiente
 */
int menuSitesManagement() {
    int option;
    printf("\n-------Gestión de Sitios de Eventos-------\n");
    printf("1. Cargar archivo de sitios\n");
    printf("2. Editar sitios\n");
    printf("3. Eliminar sitio\n");
    printf("4. Mostrar sitios\n");
    printf("0. Volver\n");
    printf("> ");

    while (scanf("%d", &option) != 1) {
        while (getchar() != '\n'); //Limpia el buffer si la entrada no es válida
        printf("Opción no válida, intente de nuevo: ");
    }
    limpiarBuffer();
    return option;
}

/*
 * siteExists
 *
 * Objetivo: Verificar que el sitio exista en el sistema
 *
 * Entradas: const char *nombre - Es el nombre del sitio que está buscando
 *
 * Salidas: int - Retorna 1 si el sitio existe, de lo contario es 0
 *
 * Restricciones: Sitios debe estar inicializado
 */
static int siteExists(const char *nombre) {
    for (int i = 0; i < numSitios; i++) {
        if (strcmp(sitios[i].nombre, nombre) == 0) { //Compara los nombres de los sitios existentes con el buscado
            return 1;
        }
    }
    return 0;
}

/*
 * findSiteIndexByName
 *
 * Objetivo: Encontrar el indice dentro del arreglo del sitio buscado
 *
 * Entradas: const char *nombre - Es el nombre del sitio que está buscando
 *
 * Salidas: int - retorna el índice del sitio dentro del arreglo, o -1 si no se encuentra
 *
 * Restricciones: Sitios debe estar inicializado
 */
int findSiteIndexByName(const char *nombre) {
    for (int i = 0; i < numSitios; i++) {
        if (strcmp(sitios[i].nombre, nombre) == 0) {
            return i;
        }
    }
    return -1;
}

/*
 * trimWhitespace
 *
 * Objetivo: Eliminar espacios en blanco dentro de una linea
 *
 * Entradas: char *str - Representa la linea que se está procesando
 *
 * Salidas: void
 *
 * Restricciones: Sin restricciones específicas
 */
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
        str[j++] = str[i]; //Coloca la nueva linea
    }
    str[j] = '\0';
}

/*
 * limpiarBuffer
 *
 * Objetivo: Libera buffer después de un scanf
 *
 * Entradas: Sin entradas
 *
 * Salidas: void
 *
 * Restricciones: Sin restricciones específicas
 */
static void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/*
 * cargarSitiosDesdeArchivo
 *
 * Objetivo: Entrar al archivo de sitios en modo lectura y cargarlos al arreglo
 *
 * Entradas: const char *rutaArchivo - Representa la ruta del archivo para lectura
 *           int esInicial - Corresponde a un int booleano (0, 1) para describir el estado
 * de ejecución actual, diferencia si se está cargando un nuevo archivo o si se está iniciando
 * el sistema
 *
 * Salidas: void
 *
 * Restricciones: Se espera que la ruta de archivo sea válida
 */
static void cargarSitiosDesdeArchivo(const char *rutaArchivo, int esInicial) {
    FILE *archivo;
    char linea[MAX_LINEA];
    char nombre[MAX_NOMBRE], ubicacion[MAX_UBICACION], web[MAX_WEB];
    int resultado; //Linea parseada
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

        //Limpia saltos de linea
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

        //Limpia espacios en blanco en los valores
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
            struct Site *temp = realloc(sitios, capacidad * sizeof(struct Site)); //Crea puntero temporal
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

    //Reporte de resultados
    if (!esInicial) {
        printf("\n--- Resumen de carga ---\n");
        printf("Sitios agregados: %d\n", sitiosAgregados);
        printf("Sitios duplicados (ignorados): %d\n", sitiosDuplicados);
        printf("Total de sitios en el sistema: %d\n", numSitios);
    }
}

/*
 * saveFile
 *
 * Objetivo: Tomar los valores en el arreglo y escribirlos en el archivo
 *
 * Entradas: Sin entradas
 *
 * Salidas: void
 *
 * Restricciones: Sin restricciones específicas
 */
void saveFile() {
    FILE *archivo;

    archivo = fopen(RUTA_SITIOS, "w");
    if (archivo == NULL) {
        printf("Error: No se pudo guardar el archivo de sitios\n");
        return;
    }

    // Escribe cada sitio en formato: nombre,ubicacion,web
    for (int i = 0; i < numSitios; i++) {
        fprintf(archivo, "%s,%s,%s\n", sitios[i].nombre, sitios[i].ubicacion, sitios[i].web);
    }

    fclose(archivo);
}

/*
 * loadInitialSites
 *
 * Objetivo: Invoca cargarSitiosDesdeArchivo
 *
 * Entradas: Sin entradas
 *
 * Salidas: void
 *
 * Restricciones: Sin restricciones específicas
 */
void loadInitialSites() {
    cargarSitiosDesdeArchivo(RUTA_SITIOS, 1);
}

/*
 * loadFile
 *
 * Objetivo: Carga un nuevo archivo de sitios para ingresarlos al sistema
 *
 * Entradas: rutaArchivo - Valor ingresado por el usuario que representa la dirección de
 * acceso al archivo
 *
 * Salidas: void
 *
 * Restricciones: Se espera que la ruta de archivo exista en el equipo o fichero
 */
void loadFile() {
    char rutaArchivo[MAX_RUTA];

    // Validación de ruta no vacía
    do {
        printf("Ingrese la dirección del archivo que desea cargar: ");
        fgets(rutaArchivo, sizeof(rutaArchivo), stdin);
        if (rutaArchivo[strlen(rutaArchivo) - 1] == '\n') {
            rutaArchivo[strlen(rutaArchivo) - 1] = '\0';
        }
        if (strlen(rutaArchivo) == 0) {
            printf("La ruta no puede estar vacía.\n");
        }
    } while (strlen(rutaArchivo) == 0);

    cargarSitiosDesdeArchivo(rutaArchivo, 0);

    saveFile();
}

/*
 * editSite
 *
 * Objetivo: Ingresar a un sitio específico y modificar sus valores
 *
 * Entradas: siteName - Valor ingresado por el usuario para referencias el sitio elegido
 *           newValue - Nuevo valor que será ingresado por el usuario para el atributo seleccionado
 *
 * Salidas: void
 *
 * Restricciones: Se espera que no queden espacios en blanco
 */
void editSite() {
    if (numSitios == 0) {
        printf("\nNo hay sitios disponibles para editar.\n");
        return;
    }

    char siteName[MAX_NOMBRE];
    int siteIndex;
    int optEdit;
    char newValue[MAX_NOMBRE];

    // Validación de nombre no vacío
    do {
        printf("\nIngrese el nombre del sitio a modificar: ");
        fgets(siteName, sizeof(siteName), stdin);
        if (siteName[strlen(siteName) - 1] == '\n') {
            siteName[strlen(siteName) - 1] = '\0';
        }
        trimWhitespace(siteName);
        if (strlen(siteName) == 0) {
            printf("El nombre no puede estar vacío.\n");
        }
    } while (strlen(siteName) == 0);

    siteIndex = findSiteIndexByName(siteName); //Busca el índice del sitio

    if (siteIndex == -1) {
        printf("\nError: No se encontró el sitio '%s'.\n", siteName);
        return;
    }

    //Se imprime la información del sitio
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

    while (scanf("%d", &optEdit) != 1) {
        while (getchar() != '\n'); //Limpia el buffer si la entrada no es válida
        printf("Opción no válida, intente de nuevo: ");
    }
    limpiarBuffer();

    switch (optEdit) {
        case 1:
            do {
                printf("Ingrese el nuevo nombre: ");
                fgets(newValue, sizeof(newValue), stdin);
                if (newValue[strlen(newValue) - 1] == '\n') {
                    newValue[strlen(newValue) - 1] = '\0';
                }
                trimWhitespace(newValue); //Elimina espacios blancos en el nuevo valor
                if (strlen(newValue) == 0) {
                    printf("Error: El nombre no puede estar vacío.\n");
                }
            } while (strlen(newValue) == 0);

            strcpy(sitios[siteIndex].nombre, newValue); //Copia el nuevo valor en el lugar correspondiente
            printf("Nombre actualizado correctamente.\n");
            break;

        case 2:
            do {
                printf("Ingrese la nueva ubicación: ");
                fgets(newValue, sizeof(newValue), stdin);
                if (newValue[strlen(newValue) - 1] == '\n') {
                    newValue[strlen(newValue) - 1] = '\0';
                }
                trimWhitespace(newValue);
                if (strlen(newValue) == 0) {
                    printf("Error: La ubicación no puede estar vacía.\n");
                }
            } while (strlen(newValue) == 0);

            strcpy(sitios[siteIndex].ubicacion, newValue);
            printf("Ubicación actualizada correctamente.\n");
            break;

        case 3:
            printf("Ingrese el nuevo sitio web: ");
            fgets(newValue, sizeof(newValue), stdin);
            if (newValue[strlen(newValue) - 1] == '\n') {
                newValue[strlen(newValue) - 1] = '\0';
            }
            trimWhitespace(newValue);
            strcpy(sitios[siteIndex].web, newValue); //El sitio web sí puede quedar vacío
            printf("Sitio web actualizado correctamente.\n");
            break;

        case 4:
            printf("Operación cancelada.\n");
            return;

        default:
            printf("Opción no válida.\n");
            return;
    }

    saveFile(); //Guarda los nuevos datos
}

/*
 * deleteSite
 *
 * Objetivo: Encontrar el sitio seleccionado y eliminarlo del archivo txt y del arreglo actual
 *
 * Entradas: siteName - Valor ingresado por el usuario para referencias el sitio elegido
 *
 * Salidas: void
 *
 * Restricciones: Sin restricciones específicas
 */
void deleteSite() {
    if (numSitios == 0) {
        printf("\nNo hay sitios disponibles para eliminar.\n");
        return;
    }

    char siteName[MAX_NOMBRE];
    int siteIndex;
    int confirm;

    // Validación de nombre no vacío
    do {
        printf("\nIngrese el nombre del sitio a eliminar: ");
        fgets(siteName, sizeof(siteName), stdin);
        if (siteName[strlen(siteName) - 1] == '\n') {
            siteName[strlen(siteName) - 1] = '\0';
        }
        trimWhitespace(siteName);
        if (strlen(siteName) == 0) {
            printf("El nombre no puede estar vacío.\n");
        }
    } while (strlen(siteName) == 0);

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
    printf("Desea eliminar este sitio?\n");
    printf("1. Confirmar eliminación\n");
    printf("2. Cancelar\n");
    printf("> ");

    while (scanf("%d", &confirm) != 1) {
        while (getchar() != '\n'); //Limpia el buffer si la entrada no es válida
        printf("Opción no válida, intente de nuevo: ");
    }
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

/*
 * showSites
 *
 * Objetivo: Recorre el arreglo de sitios y los imprime de uno en uno
 *
 * Entradas: sin entradas
 *
 * Salidas: void
 *
 * Restricciones: Sin restricciones específicas
 */
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

/*
 * manageSitesMenu
 *
 * Objetivo: Maneja el menú de opciones de sitios
 *
 * Entradas: option - Valor ingresado por el usuario para elegir una opcion
 *
 * Salidas: void
 *
 * Restricciones: Se espera un valor válido en las opciones
 */
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
        getchar(); // Espera a que el usuario presione Enter
    }
}