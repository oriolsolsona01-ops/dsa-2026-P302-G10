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

FILE* ask_map ();
PlaceNode* add_Place(PlaceNode* head, char* PlaceName, char* general_id, char* specific_id, double lat, double lon);
PlaceNode* fill_linkedlist (FILE *fitxer);
PlaceNode* find_Place(PlaceNode* head, char* target_place);
void input_originposition(PlaceNode* head);
