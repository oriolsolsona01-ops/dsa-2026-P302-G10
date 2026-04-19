#include <stdio.h>
#include "houses.h"
#include "sample_lib.h"

HouseNode* triar_num(HouseNode* head, char *street_name, int num){
    printf("El num %d no existeix a %s.\n", num, street_name);
    printf("Nums disponibles: \n");

    // Normalitzem el nom del carrer
    char lower_target[100];
    char expanded_target[150];

    to_lowercase(street_name, lower_target);
    expand_abbreviations(lower_target, expanded_target);

    // Mostrem tots els nums disponibles
    HouseNode* current = head;
    while (current != NULL){
        
        // Per cada casa, normalitzem el seu nom també per comparar-lo correctament
        char lower_cur[100];
        char expanded_cur[150];

        to_lowercase(current->street_name, lower_cur);
        expand_abbreviations(lower_cur, expanded_cur);
        
        // Mirem si coincideix i, en cas de que si, presentem per pantalla el número d'aquesta
        if (LevenshteinDistance(expanded_cur, expanded_target) == 0){
            printf(" [%d]\n", current->house_number);
        }
        current = current->next;
    }

    // Una vegada mostrats tots els números possibles, fem escollir un
    int choice;
    printf("Tria un número: ");
    scanf("%d", &choice);

    // Una vegada escollit, tornem a reccorrer la llista i retornem el que té el número escollit
    HouseNode* current = head;
    while (current != NULL){
        //normalitzem els noms per comparar de nou
        char lower_cur[100];
        char expanded_cur[150];

        to_lowercase(current->street_name, lower_cur);
        expand_abbreviations(lower_cur, expanded_cur);

        // Quan trobem la coïncidència EXACTA la retornem
        if (LevenshteinDistance(expanded_cur, expanded_target) == 0 && current->house_number == choice) {
            return current;
        }
        current = current->next;
    }

    // En cas de no trobar cap coïncidència, diem que el número no es vàlid
    printf("Número invàlid.\n");
    return NULL;
}
