#include "places.h"
#include "sample_lib.h"
#define MAX_LEN 150

FILE* open_map_places(char* mapa){ 
    // tasca 1 + nota 1 (la funció retorna el punter al document o NULL)
    // xs_1, xs_2, md_1, lg_1, xl_1 or 2xl_1

    //creem un string amb la ruta sencera del filename
    char rutafinal [MAX_LEN];
    sprintf(rutafinal, "maps/%s/places.txt", mapa); //escriu tot el contingut a rutafinal
    
    FILE *fitxer = fopen(rutafinal, "r");
    if (fitxer == NULL) {
        printf("Error: No file found.\n");
        return NULL;
    }

    return fitxer; 
}

PlaceNode* add_Place(PlaceNode* head, char* identifier, char* name, char* id, double lat, double lon) {
    
    PlaceNode* newPlace = (PlaceNode*)malloc(sizeof(PlaceNode));
    
    // guardem el nom i la cadena de càracters que identifica el lloc
    strcpy(newPlace->PlaceName, name);
    strcpy(newPlace->identifier, identifier);

    // i ara guardem els identificadors del tipus de lloc a una cadena que guarda el general i l'altra que guarda l'especific
    char* split = strchr(id, ':');
    if (split != NULL){                                        // en cas que existeixi el ":" que separa els tipus de llocs
        int len_1 = split - id;                                // mirem la distancia des de l'inici fins al ":"
        strncpy(newPlace->general_id, id, len_1);              // ho copiem a general_id
        newPlace->general_id[len_1] = '\0';                    // i afegim '\0' al final
        strcpy(newPlace->specific_id, split+1);                // finalment copiem el que queda a specific_id
    }
    else{                                                      // d'altra banda, si no hi ha cap ":"
        strcpy(newPlace->general_id, id);                      // copiem tot l'identifiacor a general_id
        strcpy(newPlace->specific_id, "");                     // i deixem specific_id buit
    }

    newPlace->lat = lat;
    newPlace->lon = lon;
    newPlace->next = head;

    return newPlace;
}

PlaceNode* fill_linked_list_places (FILE *fitxer){ 
    //cridem a la funció add Place llegeix la info del document i la posa cada node
    char identifier[MAX_LEN];
    char name[MAX_LEN];
    char id[MAX_LEN];
    double lat;
    double lon;
    PlaceNode* Places = NULL;

    while (fscanf(fitxer, "%[^,],%[^,],%[^,],%lf,%lf\n",identifier, name, id, &lat, &lon) == 5) {

        Places = add_Place(Places, identifier, name, id, lat, lon); 
        
    }
    return Places;
}

PlaceNode* find_place(PlaceNode* head, char* target_place) {
    PlaceNode* current = head;
    
    PlaceNode* top_3 [3] = {NULL, NULL, NULL};
    int dist_3[3] = {999999, 999999, 999999};
    
    while (current != NULL) {

        // Calculem la distància d'ortografia
        int dist = LevenshteinDistance(current->PlaceName, target_place);

        // Si és una coincidència PERFECTA (distància 0) i el número quadra
        if (dist == 0) {
            return current;
        }
        else{
            //mirem si el place entra dins del top 3
            for (int i = 0; i < 3; i++){
                if (dist < dist_3[i]){
                    // desplaçem els pitjors casos avall per fer lloc
                    for (int j = 2; j > i; j--){
                        dist_3[j] = dist_3[j-1];
                        top_3[j] = top_3[j-1];
                    }
                    // i insertem el nou
                    dist_3[i] = dist;
                    top_3[i] = current;
                    break;
                }
            }
        }
        current = current->next;
    }
    if (dist_3[0] > 15){
        return NULL;
    }
    //demanem a l'usuari que esculli el place al que està
    int opcio;
    printf ("You are in:\n");
    printf ("1. %s.\n", top_3[0]->PlaceName);
    printf ("2. %s.\n", top_3[1]->PlaceName);
    printf ("3. %s.\n", top_3[2]->PlaceName);
    scanf("%d", &opcio);

    // comprovem que la opcio és vàlida
    if (opcio < 1 || opcio > 3){
        printf ("Error! Choose a valid option.");
        return NULL;
    }
    // si la opcio és vàlida retornem el place que hagi escollit l'usuari
    else return top_3[opcio-1];
}

void free_linked_places(PlaceNode* head) {
    while (head != NULL) {
        PlaceNode* temp = head;
        head = head->next;
        free(temp);
    }
}