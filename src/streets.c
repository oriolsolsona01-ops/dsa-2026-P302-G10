#include "streets.h"
#define MAX_LEN 150

FILE* open_map_streets(char* mapa){
    char rutafinal [MAX_LEN];
    sprintf(rutafinal, "maps/%s/streets.txt", mapa);
    
    FILE *fitxer = fopen(rutafinal, "r");
    if (fitxer == NULL) {
        printf("Error: No file found.\n");
        return NULL;
    }
    return fitxer; 
}

StreetNode* add_street(StreetNode* head, int from, double from_lat, double from_lon, 
int to, double to_lat, double to_lon, double len, char* name){

    StreetNode* newStreet = (StreetNode*)malloc(sizeof(StreetNode));
    // afegim a la nova street les dades de la intersecció inicial
    newStreet->carrer.from_id = from;
    newStreet->carrer.from_lat = from_lat;
    newStreet->carrer.from_lon = from_lon;
    // les de la intersecció final
    newStreet->carrer.to_id = to;
    newStreet->carrer.to_lat = to_lat;
    newStreet->carrer.to_lon = to_lon;

    strcpy(newStreet->carrer.street_name, name);

}