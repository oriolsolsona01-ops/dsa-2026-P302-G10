#include <time.h>
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
        if (map_file == NULL) return NULL;

        HouseNode* list_of_houses = fill_linked_list_houses(map_file);
        fclose(map_file);

        printf("Enter street name (e.g. \"Carrer de Roc Boronat\"): ");
        scanf(" %[^\n]", street_name);

        HouseNode* trobat = find_house_name(list_of_houses, street_name);
        if (trobat == NULL) return NULL;
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
        if (map_file == NULL) return NULL;

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
    return NULL;
}
Position* input_destinationposition(char* mapa){ 

    int posicio_destí;
    int num;
    char street_name[100];
    char place_name[100];

    printf("\n--- DESTINATION ---\n");
    printf("Where do you want to go? Address (1), Place (2) or Coordinate (3): ");
    scanf("%d", &posicio_destí);

    while (posicio_destí < 1 || posicio_destí > 3){
        // netejem posicio_desti
        while(getchar() != '\n');
        // i tornem a preguntar
        printf("Where do you want to go? Address (1), Place (2) or Coordinate (3): ");
        scanf("%d", &posicio_destí);
    }
    if (posicio_destí == 1){

        FILE* map_file = open_map_house(mapa);
        if (map_file == NULL) return NULL;

        HouseNode* list_of_houses = fill_linked_list_houses(map_file);
        fclose(map_file);

        printf("Enter street name (e.g. \"Carrer de Roc Boronat\"): ");
        scanf(" %[^\n]", street_name);

        HouseNode* trobat = find_house_name(list_of_houses, street_name);
        if (trobat == NULL) return NULL;
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
    else if(posicio_destí == 2){
        FILE* map_file = open_map_places(mapa);
        if (map_file == NULL) return NULL;

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
    return NULL;
}
//**********HELPER FUNCTION**********
void show_connected_streets(Hash_map* hash_map, Street* closest_street, StreetNode* list_of_streets) {
    
    // --- MESURA SEQÜENCIAL (Lab 4) ---
    clock_t start_seq = clock();
    StreetNode* current = list_of_streets;
    while (current != NULL) {
        if (current->carrer.from_id == closest_street->to_id) {
            // carrer connectat trobat
        }
        current = current->next;
    }
    clock_t end_seq = clock();
    double ms_seq = ((double)(end_seq - start_seq) / CLOCKS_PER_SEC) * 1000.0;
    printf("[MESURA] Seqüencial: %.4f ms\n", ms_seq);

    // --- MESURA HASHMAP (Lab 5) ---
    clock_t start_hash = clock();
    get_streets_at_intersection(hash_map, closest_street->to_id);
    clock_t end_hash = clock();
    double ms_hash = ((double)(end_hash - start_hash) / CLOCKS_PER_SEC) * 1000.0;
    printf("[MESURA] Hashmap: %.4f ms\n", ms_hash);

    // --- SORTIDA NORMAL ---
    printf("From this street segment, you can go to:\n");
    printf("    - %s\n", closest_street->street_name);
    printf("        Which is connected to:\n");
    StreetNode* connected_1 = get_streets_at_intersection(hash_map, closest_street->from_id);
    while (connected_1 != NULL) {
        printf("         - %s\n", connected_1->carrer.street_name);
        connected_1 = connected_1->next;
    }
    StreetNode* connected_2 = get_streets_at_intersection(hash_map, closest_street->to_id);
    while (connected_2 != NULL) {
        printf("         - %s\n", connected_2->carrer.street_name);
        connected_2 = connected_2->next;
    }
} 

Street* input_closest_street(Position* posicio_origen, StreetNode* list_of_streets){
    // trobem el carrer més proper
    Street* closest_street = find_closest_street(posicio_origen,list_of_streets);
    // i mostem el text per pantalla
    printf("Closest street: %s\n", closest_street->street_name);
    printf("Between %lld (%lf, %lf) and %lld (%lf, %lf)\n", closest_street->from_id, closest_street->from_position.lat, closest_street->from_position.lon, closest_street->to_id, closest_street->to_position.lat, closest_street->to_position.lon);
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

StreetNode* BFS(Hash_map* intersections_graph, Street* fromStreet, Street* toStreet);

int main() {
    printf("**************************************************\n");
    printf("   EXECUTANT MESURES AUTOMÀTIQUES PER AL REPORT    \n");
    printf("**************************************************\n");

    // ---- TAULES 2.1 i 2.2 ----
    char* mapes[] = {"maps/xs_2/streets.txt", "maps/md_1/streets.txt", "maps/lg_1/streets.txt", "maps/xl_1/streets.txt"};
    char* noms_taula[] = {"xs_2", "md_1", "lg_1", "xl_1"};
    
    for(int i = 0; i < 4; i++) {
        char* ruta_mapa = mapes[i];
        char* nom_mapa = noms_taula[i];

        FILE* street_file = fopen(ruta_mapa, "r");
        if (street_file == NULL) { printf("[MAPA %s] No trobat\n", nom_mapa); continue; }
        StreetNode* list_of_streets = fill_linked_streets(street_file);
        fclose(street_file);
        if (list_of_streets == NULL) continue;

        Hash_map* map = fill_hashmap_from_streets(list_of_streets, 8192);
        Street* test_street = &(list_of_streets->carrer);
        StreetNode* temp = list_of_streets;
        while (temp->next != NULL) temp = temp->next;
        Street* to_street = &(temp->carrer);

        clock_t start_seq = clock();
        StreetNode* current = list_of_streets;
        while (current != NULL) { if (current->carrer.from_id == test_street->to_id) { } current = current->next; }
        clock_t end_seq = clock();
        double temps_seq = ((double)(end_seq - start_seq)/CLOCKS_PER_SEC)*1000.0;

        double temps_hash = 0.0;
        if (map != NULL) {
            clock_t start_hash = clock();
            get_streets_at_intersection(map, test_street->to_id);
            clock_t end_hash = clock();
            temps_hash = ((double)(end_hash - start_hash)/CLOCKS_PER_SEC)*1000.0;
        }
        printf("[TAULA 2.1] Mapa: %s | Seqüencial: %.4f ms | Hashmap: %.4f ms\n", nom_mapa, temps_seq, temps_hash);

        clock_t start_bfs_hash = clock();
        StreetNode* cami_hash = BFS(map, test_street, to_street);
        clock_t end_bfs_hash = clock();
        double temps_bfs_hash = ((double)(end_bfs_hash - start_bfs_hash)/CLOCKS_PER_SEC)*1000.0;
        if (cami_hash != NULL) free(cami_hash);

        Hash_map* map_seq = fill_hashmap_from_streets(list_of_streets, 1);
        clock_t start_bfs_seq = clock();
        StreetNode* cami_seq = BFS(map_seq, test_street, to_street);
        clock_t end_bfs_seq = clock();
        double temps_bfs_seq = ((double)(end_bfs_seq - start_bfs_seq)/CLOCKS_PER_SEC)*1000.0;
        if (cami_seq != NULL) free(cami_seq);
        free_hashmap(map_seq);
        printf("[TAULA 2.2] Mapa: %s | BFS seqüencial: %.4f ms | BFS hashmap: %.4f ms\n", nom_mapa, temps_bfs_seq, temps_bfs_hash);

        if (map != NULL) free_hashmap(map);
    }

    // ---- TAULA 2.3: BFS vs DISTÀNCIA (mapa xl_1) ----
    printf("\n--- TAULA 2.3: BFS vs distància (mapa xl_1) ---\n");

    FILE* sf = fopen("maps/xl_1/streets.txt", "r");
    if (sf == NULL) { printf("No s'ha trobat xl_1\n"); return 0; }
    StreetNode* streets_xl = fill_linked_streets(sf);
    fclose(sf);

   

    Hash_map* map_xl = fill_hashmap_from_streets(streets_xl, 8192);
    Hash_map* map_xl_seq = fill_hashmap_from_streets(streets_xl, 1);

    // 4 parelles de llocs a distàncies creixents
// 4 parelles de coordenades a distàncies creixents des de UPF
double orígens_lat[] = {41.403782, 41.403782, 41.403782, 41.403782};
double orígens_lon[] = {2.193446,  2.193446,  2.193446,  2.193446};
double destins_lat[]  = {41.403565, 41.397530, 41.403530, 41.389559};
double destins_lon[]  = {2.186490,  2.180820,  2.153870,  2.135112};
char* etiquetes[]     = {"~500m (UPF -> Parc Ciutadella)", "~2km (UPF -> Estacio de Francia)", "~5km (UPF -> Glories)", "~10km (UPF -> L'Illa Diagonal)"};

for (int i = 0; i < 4; i++) {
    Position pos_origen = {orígens_lat[i], orígens_lon[i]};
    Position pos_desti  = {destins_lat[i], destins_lon[i]};

    Street* from_street = find_closest_street(&pos_origen, streets_xl);
    Street* to_street   = find_closest_street(&pos_desti,  streets_xl);

    if (from_street == NULL || to_street == NULL) {
        printf("[TAULA 2.3] %s: carrer no trobat\n", etiquetes[i]);
        continue;
    }

    clock_t s1 = clock();
    StreetNode* c1 = BFS(map_xl, from_street, to_street);
    clock_t e1 = clock();
    double t_hash = ((double)(e1 - s1)/CLOCKS_PER_SEC)*1000.0;
    if (c1 != NULL) free(c1);

    clock_t s2 = clock();
    StreetNode* c2 = BFS(map_xl_seq, from_street, to_street);
    clock_t e2 = clock();
    double t_seq = ((double)(e2 - s2)/CLOCKS_PER_SEC)*1000.0;
    if (c2 != NULL) free(c2);

    printf("[TAULA 2.3] %s | BFS seq: %.4f ms | BFS hash: %.4f ms\n", etiquetes[i], t_seq, t_hash);
}

    free_hashmap(map_xl);
    free_hashmap(map_xl_seq);

    printf("\n--- PROVES COMPLETADES ---\n");
    return 0;
}