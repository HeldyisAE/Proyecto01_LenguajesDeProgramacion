#ifndef SITEMANAGEMENT_H
#define SITEMANAGEMENT_H

struct Site {
    char nombre[50];
    char ubicacion[50];
    char web[50];
    struct Sector *sectores; // Puntero para la memoria dinámica
    int numSectores;         // Contador de cuántos sectores tiene el sitio
};

extern struct Site *sitios;
extern int numSitios;

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