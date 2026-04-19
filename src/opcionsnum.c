#include <stdio.h>
#include "houses.h"
#include "sample_lib.h"

void input_originposition(HouseNode* head) {
    int posicio_origen;
    int num;
    char street_name[100];

    printf("\n--- ORIGIN ---\n");
    printf("Where are you? Address (1), Place (2) or Coordinate (3): ");
    scanf("%d", &posicio_origen);

    while (posicio_origen < 1 || posicio_origen > 3) {
        printf("Invalid option. Where are you? Address (1), Place (2) or Coordinate (3): ");
        scanf("%d", &posicio_origen);
    }

    if (posicio_origen == 2 || posicio_origen == 3) {
        printf("Not implemented yet.\n");
    } else {
        printf("Enter street name (e.g. \"Carrer de Roc Boronat\"): ");
        scanf(" %[^\n]", street_name);
        printf("Enter street number: ");
        scanf("%d", &num);

        // 1. Busquem la casa amb la teva funció original
        HouseNode* trobat = find_house(head, street_name, num); // Cridem la teva funció de cerca. Aquí poden passar dues coses: que trobi la casa exacta o que ens doni la que més s'assembla (però amb un número diferent).

        // 2. Lògica de validació:
        // Si el carrer coincideix però el número no, busquem alternatives
        if (trobat != NULL && strcmp(trobat->street_name, street_name) == 0 && trobat->house_number != num) {
            
            printf("\nEl número %d no existeix a %s.\n", num, street_name);
            
            // Creem la llista de números disponibles
            HouseNode* opcions = get_street_options(head, street_name);
            
            if (opcions != NULL) {
                printf("Números disponibles: ");
                HouseNode* temp = opcions;
                while (temp != NULL) {
                    printf("[%d] ", temp->house_number);
                    temp = temp->next;
                }
                
                printf("\nTria un número de la llista: ");
                scanf("%d", &num);
                
                // Tornem a buscar el node definitiu amb el número triat
                trobat = find_house(head, street_name, num);
                
                // Alliberem la llista temporal de suggeriments (ja no ens cal)
                free_list(opcions);
            }
        }

        // 3. Resultat final
        if (trobat != NULL && trobat->house_number == num) {
            printf("Found at (%f, %f)\n", trobat->lat, trobat->lon);
        } else {
            printf("Address not found.\n");
        }
    }
}


HouseNode* get_street_options(HouseNode* head, char* target_street) {
    HouseNode* options_list = NULL; // Creem el punter d'inici per a la nostra llista de suggeriments. Al principi està buida.
    HouseNode* current = head; // Creem un punter auxiliar per recórrer la llista principal (la gran) sense perdre la referència de l'inici.

    while (current != NULL) { // Comencem un bucle que anirà node per node fins que s'acabi la llista.
        // Si el nom del carrer coincideix al 100%
        if (strcmp(current->street_name, target_street) == 0) {
            // Reutilitzem la teva funció add_house per afegir el node a la llista d'opcions
            options_list = add_house(options_list, current->street_name, current->house_number, current->lat, current->lon);
        }
        current = current->next; // Saltem al següent node de la llista gran
    }
    return options_list;
}