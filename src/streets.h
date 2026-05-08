#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

// estructura d'un carrer
typedef struct Street{
    // dades de la posició inicial del carrer
    int from_id;                // from_intersaction_id, 
    double from_lat;            // from_intersection_lat,
    double from_lon;            // from_intersection_lon, 
    
    // dades de la posició final del carrer
    int to_id;                  // to_intersection_id, 
    double to_lat;              // to_intersection_lat, 
    double to_lon;              // to_intersection_lon, 

    // dades generals del carrer
    double lenght;
    char* street_name[100];
} Street;

// estructura d'un node d'un carrer
typedef struct StreetNode{
    struct Street carrer;
    StreetNode* next;
} StreetNode;

FILE* open_map_streets(char* mapa);
StreetNode* add_street(StreetNode* head, int from, double from_lat, double from_lon, int to, double to_lat, double to_lon, double len, char* name){
// falten funcions de streets
