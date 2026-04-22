#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_LENGTH 100

// Definim el node per a la llista de llocs
typedef struct PlaceNode {
    char identifier[MAX_LENGTH];
    char PlaceName[MAX_LENGTH];
    char general_id[MAX_LENGTH];
    char specific_id[MAX_LENGTH];
    double lat;
    double lon;
    struct PlaceNode* next; // Punter al següent node
} PlaceNode;

FILE* open_map_places();
PlaceNode* add_Place(PlaceNode* head, char* PlaceName, char* general_id, char* specific_id, double lat, double lon);
PlaceNode* fill_linked_list_places (FILE *fitxer);
PlaceNode* find_place(PlaceNode* head, char* target_place);
