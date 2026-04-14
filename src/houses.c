#include "houses.h"
#include "sample_lib.h"

HouseNode* add_house(HouseNode* head, char* street, int number, double lat, double lon) {
    HouseNode* new_house = (HouseNode*)malloc(sizeof(HouseNode));
    
    strcpy(new_house->street_name,street);
    new_house->house_number = number;
    new_house->lat = lat;
    new_house->lon = lon;

    new_house->next = head;

    return new_house;
}

HouseNode* find_house(HouseNode* head, char* target_street, int target_number) {
    HouseNode* current = head;
    
    HouseNode* best_match = NULL;
    int min_distance = 999999;

    while (current != NULL) {
        // Calculem la distància d'ortografia
        int dist = LevenstheinDistance(current->street_name, target_street);

        // Si és una coincidència PERFECTA (distància 0) i el número quadra
        if (dist == 0 && current->house_number == target_number) {
            return current;
        }

        // Si la distància és menor que el nostre rècord actual, actualitzem el rècord
        // (Nota: per ara ignorem el número de casa aquí, busquem el carrer més semblant)
        if (dist < min_distance) {
            min_distance = dist;
            best_match = current;
        }

        current = current->next;
    }

    // Si hem acabat el bucle i no hem trobat una coincidència perfecta (0),
    // retornem la casa del carrer que s'assemblava més
    return best_match;
}