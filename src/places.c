#include "places.h"
#include "sample_lib.h"
#define MAX_LEN 150

FILE* ask_map_places (){ 
    // tasca 1 + nota 1 (la funció retorna el punter al document o NULL)
    // xs_1, xs_2, md_1, lg_1, xl_1 or 2xl_1
    char mapa[MAX_LEN];
    
    printf("Introdueix un nom de mapa (xs_1, xs_2, md_1, lg_1, xl_1 or 2xl_1): ");
    scanf("%s", mapa);

    //creem un string amb la ruta sencera del filename
    char rutafinal [MAX_LEN];
    sprintf(rutafinal, "maps/%s/places.txt", mapa); //escriu tot el contingut a rutafinal
    
    FILE *fitxer = fopen(rutafinal, "r");
    if (fitxer == NULL) {
        printf("Error: No s'ha pogut obrir el fitxer.\n");
        return NULL;
    }

        return fitxer; 
}

PlaceNode* add_Place(PlaceNode* head, char* identifier, char* name, char* id, double lat, double lon) {
    PlaceNode* new_Place = (PlaceNode*)malloc(sizeof(PlaceNode));
    
    // guardem el nom i la cadena de càracters que identifica el lloc
    strcpy(new_Place->PlaceName, name);
    strcpy(new_Place->identifier, identifier);

    // i ara guardem els identificadors del tipus de lloc a una cadena que guarda el general i l'altra que guarda l'especific
    char* split = strchr(identifier, ':');
    if (split != NULL){                                        // en cas que existeixi el ":" que separa els tipus de llocs
        int len_1 = split - identifier;                        // mirem la distancia des de l'inici fins al ":"
        strncpy(newPlace->general_id, identifier, len_1);      // ho copiem a general_id
        newPlace->general_id[len_1] = '\0';                    // i afegim '\0' al final
        strcpy(newPlace->specific_id, split+1);                // finalment copiem el que queda a specific_id
    }
    else{                                                      // d'altra banda, si no hi ha cap ":"
        strcpy(newPlace->general_id, identifier);              // copiem tot l'identifiacor a general_id
        strcpy(newPlace->specific_id, "");                     // i deixem specific_id buit
    }

    new_Place->lat = lat;
    new_Place->lon = lon;
    new_Place->next = head;

    return new_Place;
}

PlaceNode* fill_linkedlist (FILE *fitxer){ 
    //cridem a la funció add Place llegeix la info del document i la posa cada node
    char identifier[MAX_LEN];
    char name[MAX_LEN];
    char id[MAX_LEN];
    double lat;
    double lon;
    PlaceNode* Places = NULL;

    while (fscanf(fitxer, "%[^,],%d,%lf,%lf\n",identifier, name, id, &lat, &lon) == 5) {

        Places = add_Place(Places, identifier, name, id, lat, lon); 
        
    }
    return Places
}

PlaceNode* find_Place(PlaceNode* head, char* target_street, int target_number) {
    PlaceNode* current = head;
    
    PlaceNode* best_match = NULL;
    int min_distance = 999999;

    while (current != NULL) {
        // Calculem la distància d'ortografia
        int dist = LevenshteinDistance(current->street_name, target_street);

        // Si és una coincidència PERFECTA (distància 0) i el número quadra
        if (dist == 0 && current->Place_number == target_number) {
            return current;
        }

        // Si la distància és menor que el nostre rècord actual, actualitzem el rècord
        if (dist < min_distance) {
            min_distance = dist;
            best_match = current;
        }

        current = current->next;
    }

    // retornem la casa del carrer que s'assemblava més
    return best_match;
}

void input_originposition(PlaceNode* head){ // tasca 2,3 i 4 (el paràmtre d'entrada serà el punter al doc)

    int posicio_origen;
    int num;
    char street_name[100];

    printf("\n--- ORIGIN ---\n");
    printf("Where are you? Address (1), Place (2) or Coordinate (3): ");
    scanf("%d", &posicio_origen);

    while (posicio_origen < 1 || posicio_origen > 3){
        printf("Invalid option. Where are you? Address (1), Place (2) or Coordinate (3): ");
        scanf("%d", &posicio_origen);
    }
    if (posicio_origen == 2 || posicio_origen == 3){
        printf("Not implemented yet.\n");
    }
    
    else{
        printf("Enter street name (e.g. \"Carrer de Roc Boronat\"): ");
        scanf(" %[^\n]", street_name);

        printf("Enter street number (e.g. \"138\"): ");
        scanf("%d", &num);
        //cal trobar les coordenades un cop tenint aquestes dades amb el nom i num amb fscanf

        PlaceNode* trobat = find_Place(head, street_name, num);

        if (trobat != NULL) {
            printf("Found at (%f, %f)\n", trobat->lat, trobat->lon);
        } else {
            printf("Address not found.\n");
        }
    }
    
}