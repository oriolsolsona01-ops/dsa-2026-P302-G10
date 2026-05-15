#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

// Definim el node per a la llista de cases
typedef struct HouseNode {
    char street_name[100];
    int house_number;
    double lat;
    double lon;
    struct HouseNode* next; // Punter al següent node
} HouseNode;

FILE* open_map_house();
HouseNode* add_house(HouseNode* head, char* street, int number, double lat, double lon);
void to_lowercase(char* original, char* dest);
void expand_abbreviations(const char* original, char* dest);
HouseNode* fill_linked_list_houses (FILE *fitxer);
HouseNode* find_house_name(HouseNode* head, char* target_street);
HouseNode* triar_num(HouseNode* head, char *street_name, int num);

