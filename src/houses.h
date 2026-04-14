#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definim el node per a la llista de cases
typedef struct HouseNode {
    char street_name[100];
    int house_number;
    double lat;
    double lon;
    struct HouseNode* next; // Punter al següent node
} HouseNode;