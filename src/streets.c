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
                       int to, double to_lat, double to_lon, double len, char* name) {

    StreetNode* newStreet = (StreetNode*)malloc(sizeof(StreetNode));
    if (newStreet == NULL) return head; // Seguretat si falla el malloc

    // Afegim a la nova street les dades de la intersecció inicial
    newStreet->carrer.from_id = from;
    newStreet->carrer.from_lat = from_lat;
    newStreet->carrer.from_lon = from_lon;

    // Les de la intersecció final
    newStreet->carrer.to_id = to;
    newStreet->carrer.to_lat = to_lat;
    newStreet->carrer.to_lon = to_lon;
    
    newStreet->carrer.lenght = len;
    strcpy(newStreet->carrer.street_name, name);

    
    newStreet->next = head; 

    return newStreet;
}

StreetNode* fill_linked_streets(FILE *fitxer) { 
    int from, to;
    double from_lat, from_lon, to_lat, to_lon, len;
    char name[100];
    StreetNode* streets = NULL;

    while (fscanf(fitxer,"%[^,], %d,%lf,%lf,%d,%lf,%lf,%lf", 
           &from, &from_lat, &from_lon, &to, &to_lat, &to_lon, &len, name) == 8) {

        // Cridem a add_street passant-li el "cap" actual (streets)
        streets = add_street(streets, from, from_lat, from_lon, to, to_lat, to_lon, len, name); 
    }

    return streets; 
}



