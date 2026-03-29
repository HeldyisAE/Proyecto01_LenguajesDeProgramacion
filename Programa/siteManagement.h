#ifndef SITEMANAGEMENT_H
#define SITEMANAGEMENT_H

//Definición de macros
#define MAX_NOMBRE       50
#define MAX_UBICACION    50
#define MAX_WEB          50
#define MAX_LINEA        200
#define MAX_RUTA         256
#define INCREMENTO_MEMORIA  5
#define RUTA_SITIOS  "datos/sitios.txt"

//Definición de struct
struct Site {
    char nombre[50];
    char ubicacion[50];
    char web[50];
    struct Sector *sectores; // Puntero para la memoria dinámica
    int numSectores;         // Contador de cuántos sectores tiene el sitio
};

//Definiciones externas
extern struct Site *sitios;
extern int numSitios;

//Prototipos
int menuSitesManagement();
int findSiteIndexByName(const char *nombre);
void manageSitesMenu();
void loadFile();
void editSite();
void deleteSite();
void showSites();
void loadInitialSites();
void saveFile();
void savePrices();
void loadPrices();


#endif