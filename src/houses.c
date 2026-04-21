#include "houses.h"
#include "sample_lib.h"

FILE* ask_map (){ // tasca 1 + nota 1 (la funció retorna el punter al document o NULL)
    // xs_1, xs_2, md_1, lg_1, xl_1 or 2xl_1
    char mapa[50];
    
    printf("Introduce the name of the map (xs_1, xs_2, md_1, lg_1, xl_1 or 2xl_1): ");
    scanf("%s", mapa);

    //creem un string amb la ruta sencera del filename
    char rutafinal [150];
    sprintf(rutafinal, "maps/%s/houses.txt", mapa); //escriu tot el contingut a rutafinal
    
    FILE *fitxer = fopen(rutafinal, "r");
    if (fitxer == NULL) {
        printf("Error: The document could not be open.\n");
        return NULL;
    }

        return fitxer; 
}
// en la funció anterior es demana a l'usuari el mapa i s'obra el fitxer corresponent

HouseNode* add_house(HouseNode* head, char* street, int number, double lat, double lon) {
    HouseNode* new_house = (HouseNode*)malloc(sizeof(HouseNode));
    
    strcpy(new_house->street_name,street);
    new_house->house_number = number;
    new_house->lat = lat;
    new_house->lon = lon;

    new_house->next = head;

    return new_house;

    // creem un nou node de manera dinàmica i l'omplim amb la info que toqui 
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
        return houses;

        // llegeix el fitxer línia per línia i crida add_house per omplir la llista enllaçada amb totes les cases del mapa.
    }

HouseNode* find_house(HouseNode* head, char* target_street, int target_number) {
    HouseNode* current = head;
    // la funció el que va es buscar una casa en concret

    // 
    
    HouseNode* best_match = NULL;
    int min_distance = 999999;

    while (current != NULL) {
        // Calculem la distància d'ortografia
        int dist = LevenshteinDistance(current->street_name, target_street);

        // Si troba el carrer i el num que es buscava (distància Levenshtein 0) ho retorna
        // Si no, va guardant quina es la casa que te el nom de carrer més semblant (best_match) per si no hi ha cap coincidència exacta
        if (dist == 0 && current->house_number == target_number) {
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

HouseNode* get_street_options(HouseNode* head, char* target_street) {
    HouseNode* options_list = NULL; // creem el punter d'inici per a la nostra llista  incialment buida de suggeriments
    HouseNode* current = head; // Creem un punter auxiliar per recorer la llista principal sense perdre la referència de l'inici.
    // creem una llista nova només amb les cases que pertanyen a un carrer concrer per oferir alternatives quan el número de casa no coincideix amb el que buscava l'usuari

    while (current != NULL) { // Comencem un bucle que anirà node per node fins que s'acabi la llista.
        // Si el nom del carrer coincideix al 100%
        if (strcmp(current->street_name, target_street) == 0) {
            // Reutilitzem la unció add_house per afegir el node a la llista d'opcions
            options_list = add_house(options_list, current->street_name, current->house_number, current->lat, current->lon);
        }
        current = current->next; // 
    }
    return options_list;
}

void input_originposition(HouseNode* head){ // tasca 2,3 i 4 (el paràmtre d'entrada serà el punter al doc)

    int posicio_origen;
    int num;
    char street_name[100];
    // és el que veu l'usuari

    printf("Where are you? Address (1), Place (2) or Coordinate (3): ");
    scanf("%d", &posicio_origen); // ho guardem en la variable origen

    while (posicio_origen < 1 || posicio_origen > 3){
        printf("Invalid option. Where are you? Address (1), Place (2) or Coordinate (3): ");
        scanf("%d", &posicio_origen);
    }
    if (posicio_origen == 2 || posicio_origen == 3){
        printf("Not implemented yet\n");
    }
    
    else{
        printf("Enter street name (e.g. \"Carrer de Roc Boronat\"): ");
        scanf(" %[^\n]", street_name);

        printf("Enter street number (e.g. \"138\"): ");
        scanf("%d", &num);

        


HouseNode* trobat = find_house(head, street_name, num); // cridem a la funcio a partir de la info que ens dona l'usuari
//
         if (trobat != NULL && strcmp(trobat->street_name, street_name) == 0 && trobat->house_number != num) {
            
            printf("\nEl número %d no existeix a %s.\n", num, street_name);
            
            // creem una llista on posarem les opcions de les possibles cases reals
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
                
                // 
                trobat = find_house(head, street_name, num);
                
                // alliberem la memòria
                free_list(opcions);
            }
        }

        
        if (trobat != NULL && trobat->house_number == num) {
            printf("Found at (%f, %f)\n", trobat->lat, trobat->lon);
        } else {
            printf("Address not found.\n");
        }
    

    }


