#include "houses.h"
#include "sample_lib.h"

FILE* open_map_house(char mapa){ // tasca 1 + nota 1 (la funció retorna el punter al document o NULL)
    // xs_1, xs_2, md_1, lg_1, xl_1 or 2xl_1


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

HouseNode* add_house(HouseNode* head, char* street, int number, double lat, double lon) {
    HouseNode* new_house = (HouseNode*)malloc(sizeof(HouseNode));
    
    strcpy(new_house->street_name,street);
    new_house->house_number = number;
    new_house->lat = lat;
    new_house->lon = lon;

    new_house->next = head;

    return new_house;
}

HouseNode* fill_linked_list_houses (FILE *fitxer){ //cridem a la funció add house llegeix la info del document i la posa cada node
    char street[100];
    int number;
    double lat;
    double lon;
    HouseNode* houses = NULL;

    while (fscanf(fitxer, "%[^,],%d,%lf,%lf\n",street, &number, &lat, &lon) == 4) {

        houses = add_house(houses, street, number, lat, lon); 
        
    }
    return houses;
}


void to_lowercase(char* original, char* dest) {
    int i = 0;
    while (original[i] != '\0') {
        // Comprovem si és una lletra majúscula utilitzant el seu valor ASCII
        if (original[i] >= 'A' && original[i] <= 'Z') {
            dest[i] = original[i] + 32; // Sumem 32 per passar-la a minúscula
        } else {
            dest[i] = original[i]; // Si ja era minúscula, número o espai, es queda igual
        }
        i++;
    }
    dest[i] = '\0'; // Marquem el final de l'string
}

void expand_abbreviations(const char* original, char* dest) {
    // Comprovem si els primers 3 caràcters són "c. "
    if (strncmp(original, "c. ", 3) == 0) {
        strcpy(dest, "carrer ");
        strcat(dest, original + 3);
    } 
    // Comprovem Avinguda ("av. " -> 4 caràcters)
    else if (strncmp(original, "av. ", 4) == 0) {
        strcpy(dest, "avinguda ");
        strcat(dest, original + 4);
    } 
    // Comprovem Passeig ("pg. " -> 4 caràcters)
    else if (strncmp(original, "pg. ", 4) == 0) {
        strcpy(dest, "passeig ");
        strcat(dest, original + 4);
    } 
    // Comprovem Passatge ("ptge. " -> 6 caràcters)
    else if (strncmp(original, "ptge. ", 6) == 0) {
        strcpy(dest, "passatge ");
        strcat(dest, original + 6); // Saltem els 6 caràcters de "ptge. "
    } 
    // Si no comença per cap abreviatura, simplement ho copiem tal qual
    else {
        strcpy(dest, original);
    }
}

HouseNode* find_house_name(HouseNode* head, char* target_street) {
    HouseNode* current = head;
    
    HouseNode* best_match = NULL;
    int min_distance = 999999;

    char lower_street[100];
    to_lowercase(target_street,lower_street);

    char expanded_street[150];
    expand_abbreviations(lower_street,expanded_street);


    while (current != NULL) {
        char lower_current[100];
        to_lowercase(current->street_name,lower_current);

        char expanded_current[150];
        expand_abbreviations(lower_current,expanded_current);

        // Calculem la distància d'ortografia
        int dist = LevenshteinDistance(expanded_current, expanded_street);
        
        // Si és una coincidència PERFECTA (distància 0) i el número quadra
        if (dist == 0) {
            return current;
        }
        //aqui falta ficar el sorting
    }

    // sorting

}

HouseNode* triar_num(HouseNode* head, char *street_name, int num){
    
    HouseNode* current = head;
    while (current != NULL){
        // Mirem si coincideix i, en cas de que si, presentem per pantalla el número d'aquesta
        if (current->street_name == street_name && current->house_number == num ){
            return current;
        }
        current = current->next;
    }
    // si arribem aqui vol dir que cap numero coincideix
    printf("El num %d no existeix a %s.\n", num, street_name);
    printf("Nums disponibles: \n");

    while (current != NULL){
        if (current->street_name == street_name)
        printf(" [%d]\n", current->house_number);
    }

    // Una vegada mostrats tots els números possibles, fem escollir un
    int choice;
    printf("Tria un número: ");
    scanf("%d", &choice);

    // Una vegada escollit, tornem a recorrer la llista i retornem el que té el número escollit
    HouseNode* current = head;
    while (current != NULL){
        if (current->street_name == street_name && current->house_number == choice) {
            return current;
        }
        current = current->next;
    }

    // En cas de no trobar cap coïncidència, diem que el número no es vàlid
    printf("Número invàlid.\n");
    return NULL;
}