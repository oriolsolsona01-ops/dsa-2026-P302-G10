#include "sample_lib.h"
#include "houses.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

FILE* ask_map (){ // tasca 1 + nota 1 (la funció retorna el punter al document o NULL)
    // xs_1, xs_2, md_1, lg_1, xl_1 or 2xl_1
    char mapa[50];
    
    scanf("Introdueix un nom de mapa: xs_1, xs_2, md_1, lg_1, xl_1 or 2xl_1: %s", mapa);

    //creem un string amb la ruta sencera del filename
    char rutafinal [150];
    sprintf(rutafinal, "maps/%s/houses.txt", mapa); //escriu tot el contingut a rutafinal
    
    FILE *fitxer = fopen(rutafinal, "r");
    if (fitxer == NULL) {
        printf("Error: No s'ha pogut obrir el fitxer.\n");
        return NULL;
    }

        return fitxer; 
}

HouseNode* fill_linkedlist (FILE *fitxer){ //cridem a la funció add house llegeix la info del document i la posa cada node
        char street[100];
        int number;
        double lat;
        double lon;
        HouseNode* houses = NULL;

        while (fscanf(fitxer, "%[^,],%d,%lf,%lf\n",street, &number, &lat, &lon) == 4) {

            houses = add_house(houses, street, number, lat, lon); 
            
        }
    }
    
void input_originposition(HouseNode* head){ // tasca 2,3 i 4 (el paràmtre d'entrada serà el punter al doc)

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

        HouseNode* trobat = find_house(head, street_name, num);

        if (trobat != NULL) {
            printf("Found at (%f, %f)\n", trobat->lat, trobat->lon);
        } else {
            printf("Address not found.\n");
        }
    }
    
}





