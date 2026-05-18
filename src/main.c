#include "sample_lib.h"
#include "streets.h"
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

Position* input_originposition(char* mapa){ // tasca 2,3 i 4 (el paràmtre d'entrada serà el punter al doc)

    int posicio_origen;
    int num;
    char street_name[100];
    char place_name[100];

    printf("\n--- ORIGIN ---\n");
    printf("Where are you? Address (1), Place (2) or Coordinate (3): ");
    scanf("%d", &posicio_origen);

    while (posicio_origen < 1 || posicio_origen > 3){
        // netejem posicio_origen
        while(getchar() != '\n');
        // i tornem a preguntar
        printf("Invalid option. Where are you? Address (1), Place (2) or Coordinate (3): ");
        scanf("%d", &posicio_origen);
    }
    if (posicio_origen == 1){

        FILE* map_file = open_map_house(mapa);
        if (map_file == NULL) return;

        HouseNode* list_of_houses = fill_linked_list_houses(map_file);
        fclose(map_file);

        printf("Enter street name (e.g. \"Carrer de Roc Boronat\"): ");
        scanf(" %[^\n]", street_name);

        HouseNode* trobat = find_house_name(list_of_houses, street_name);
        if (trobat == NULL) return;
        else{
            printf("Enter street number (e.g. \"138\"): ");
            scanf("%d", &num);
        }

        HouseNode* final_house = triar_num(list_of_houses, trobat->street_name, num);

        if (final_house != NULL) {
            printf("Found at (%f, %f)\n", final_house->lat, final_house->lon);
        } else {
            printf("Address not found.\n");
        }

        Position* posicio_place = (Position*)malloc(sizeof(Position));
        
        posicio_place->lat = final_house->lat;
        posicio_place->lon = final_house->lon;

        return posicio_place;

    }
    else if(posicio_origen == 2){
        FILE* map_file = open_map_places(mapa);
        if (map_file == NULL) return;

        PlaceNode* list_of_places = fill_linked_list_places(map_file);
        fclose(map_file);

        printf("Enter place name (e.g. \"Universitat Pompeu Fabra–Campus del Poblenou\" or \"L'Illa Diagonal\"): ");
        scanf(" %[^\n]", place_name);

        PlaceNode* trobat = find_place(list_of_places, place_name);

        if (trobat != NULL) {
            printf("Found at (%f, %f)\n", trobat->lat, trobat->lon);
        } else {
            printf("Place not found.\n");
        }
        Position* posicio_place = (Position*)malloc(sizeof(Position));
        
        posicio_place->lat = trobat->lat;
        posicio_place->lon = trobat->lon;

        return posicio_place;
        
    }
    else{
        printf("Not implemented yet!!\n");
    }
}

Street* input_closest_street(Position* posicio_origen, char* mapa){
    FILE* street_file = open_map_streets(mapa);
        if (street_file == NULL) return;

        StreetNode* list_of_streets = fill_linked_streets(street_file);
        fclose(street_file);

        HashMap* hashmap = fill_hashmap_from_streets(list_of_streets, 1000);

        StreetNode* connected = get_streets_at_intersection(hashmap, 21638867);

        Street* closest_street = find_closest_street(posicio_origen,list_of_streets);

        printf("Closest street: %c", closest_street->street_name);
        printf("Between %d (%lf, %lf) and %d (%lf, %lf)",closest_street->from_id,closest_street->from_position.lat,closest_street->from_position.lon,closest_street->to_id,closest_street->to_position.lat,closest_street->to_position.lon);

        return closest_street;

}


char* choose_map(){
    int mapa;
    printf("Choose a map name:\n(1) -> xs_1\n(2) -> xs_2\n(3) -> md_1\n(4) -> lg_1\n(5) -> xl_1\n(6) -> 2xl_1\n");
    scanf("%d", &mapa);
    // si l'usuari no introdueix una opció vàlida:
    while (mapa != 1 && mapa != 2 && mapa != 3 && mapa != 4 && mapa != 5 && mapa != 6){
        while(getchar() != '\n');
        printf("Choose a valid option (from 1 to 6) please: ");
        scanf("%d", &mapa);
    }
    // mirem cas per cas i retornem convenientment segons la opció que hagi escollit l'usuari                  
    if (mapa == 1) return "xs_1";                                     
    else if (mapa == 2) return "xs_2";      
    else if (mapa == 3) return "md_1";
    else if (mapa == 4) return "lg_1";
    else if (mapa == 5) return "xl_1";
    else if (mapa == 6) return "2xl_1";
    // si arriba al final i no ha retornat res, retorna NULL
    return NULL;
}

int main() {

    printf("*****************\nWelcome to DSA!\n*****************\n");
    char* mapa = choose_map();
    Position* posicio_origen = input_originposition(mapa);
    Street* closest_street = input_closest_street(posicio_origen,mapa);

    return 0;

}