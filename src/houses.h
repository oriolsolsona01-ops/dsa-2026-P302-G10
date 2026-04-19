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

FILE* ask_map ();
HouseNode* add_house(HouseNode* head, char* street, int number, double lat, double lon);
void to_lowercase(char* original, char* dest);
void expand_abbreviations(const char* original, char* dest);
HouseNode* fill_linkedlist (FILE *fitxer);
HouseNode* find_house(HouseNode* head, char* target_street, int target_number);
void input_originposition(HouseNode* head);
