#include "sample_lib.h"
#include "places.h"
#include "houses.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

void createaleak() {
  char *foo = malloc(20 * sizeof(char));
  printf("Allocated leaking string: %s", foo);
}

void input_originposition(char* mapa){ // tasca 2,3 i 4 (el paràmtre d'entrada serà el punter al doc)

    int posicio_origen;
    int num;
    char street_name[100];
    char place_name[100];

    printf("\n--- ORIGIN ---\n");
    printf("Where are you? Address (1), Place (2) or Coordinate (3): ");
    scanf("%d", &posicio_origen);

    while (posicio_origen < 1 || posicio_origen > 3){
        printf("Invalid option. Where are you? Address (1), Place (2) or Coordinate (3): ");
        scanf("%d", &posicio_origen);
    }
    if (posicio_origen == 1){

        FILE* map_file = open_map_house(mapa);
        if (map_file == NULL) return 1;

        HouseNode* list_of_houses = fill_linkedlist(map_file);
        fclose(map_file);

        printf("Enter street name (e.g. \"Carrer de Roc Boronat\"): ");
        scanf(" %[^\n]", street_name);

        HouseNode* trobat = find_house_name(list_of_houses, street_name);
        
        printf("Enter street number (e.g. \"138\"): ");
        scanf("%d", &num);

        HouseNode* final_house = triar_num(list_of_houses, trobat->street_name, num);

        if (final_house != NULL) {
            printf("Found at (%f, %f)\n", trobat->lat, trobat->lon);
        } else {
            printf("Address not found.\n");
        }

    }
    else if(posicio_origen == 2){
        FILE* map_file = open_map_place(mapa);
        if (map_file == NULL) return 1;

        PlaceNode* list_of_places = fill_linkedlist(map_file);
        fclose(map_file);

        printf("Enter place name (e.g. \"Universitat Pompeu Fabra–Campus del Poblenou\" or \"L'Illa Diagonal\"): ");
        scanf(" %[^\n]", place_name);

        PlaceNode* trobat = find_place(list_of_places, place_name);

        if (trobat != NULL) {
            printf("Found at (%f, %f)\n", trobat->lat, trobat->lon);
        } else {
            printf("Place not found.\n");
        }
    }
    else{
        printf("Not implemented yet.\n"); 
    }
}


int main() {

  printf("*****************\nWelcome to DSA!\n*****************\n");
  
  
  char mapa [50];
  printf("Introdueix un nom de mapa (xs_1, xs_2, md_1, lg_1, xl_1 or 2xl_1): ");
  scanf("%s", mapa);

  input_originposition(mapa);
  

  return 0;
}