#include "streets.h"
#include <math.h>
#define MAX_LEN 150
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define EARTH_RADIUS 6371.0

FILE* open_map_streets(char* mapa){
    char rutafinal [MAX_LEN];
    sprintf(rutafinal, "maps/%s/streets.txt", mapa);
    
    FILE *fitxer = fopen(rutafinal, "r");
    if (fitxer == NULL) {
        printf("Error: No file found.\n");
        return NULL;
    }
    return fitxer; 
}

StreetNode* add_street(StreetNode* head, long long from, double from_lat, double from_lon, 
                       long long to, double to_lat, double to_lon, double len, char* name) {

    StreetNode* newStreet = (StreetNode*)malloc(sizeof(StreetNode));
    if (newStreet == NULL) return head; // Seguretat si falla el malloc

    // Afegim a la nova street les dades de la intersecció inicial
    newStreet->carrer.from_id = from;
    newStreet->carrer.from_position.lat = from_lat;
    newStreet->carrer.from_position.lon = from_lon;

    // Les de la intersecció final
    newStreet->carrer.to_id = to;
    newStreet->carrer.to_position.lat = to_lat;
    newStreet->carrer.to_position.lon = to_lon;
    
    newStreet->carrer.lenght = len;
    strcpy(newStreet->carrer.street_name, name);

    
    newStreet->next = head; 

    return newStreet;
}

StreetNode* fill_linked_streets(FILE *fitxer) { 
    long long from, to;
    double from_lat, from_lon, to_lat, to_lon, len;
    char name[100];
    StreetNode* streets = NULL;

   
while (fscanf(fitxer, "%lld,%lf,%lf,%lld,%lf,%lf,%lf,%[^\n]",
        &from, &from_lat, &from_lon, &to, &to_lat, &to_lon, &len, name) == 8) {
        //elimina el \r si existeix
        int name_len = strlen(name);
        if (name_len > 0 && name[name_len - 1] == '\r') {
            name[name_len - 1] = '\0';
        }
        // Cridem a add_street passant-li el "cap" actual (streets)
        streets = add_street(streets, from, from_lat, from_lon, to, to_lat, to_lon, len, name); 
    }

    return streets; 
}


// ********** OPERACIONS MATEMÀTIQUES **********
double toRadians(double degree) {
    return degree * (M_PI / 180.0);
}

double toDegrees(double radians) {
    return radians * (180.0 / M_PI);
}

double haversine(Position posA, Position posB) {
    double lat1 = toRadians(posA.lat);
    double lon1 = toRadians(posA.lon);
    double lat2 = toRadians(posB.lat);
    double lon2 = toRadians(posB.lon);

    double dLat = lat2 - lat1;
    double dLon = lon2 - lon1;
    double a = pow(sin(dLat / 2), 2) +
    cos(lat1) * cos(lat2) * pow(sin(dLon / 2), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return EARTH_RADIUS * c;
}

Position midpoint(Position a, Position b) {
    double lat1 = toRadians(a.lat);
    double lon1 = toRadians(a.lon);
    double lat2 = toRadians(b.lat);
    double lon2 = toRadians(b.lon);

    double x1 = cos(lat1) * cos(lon1);
    double y1 = cos(lat1) * sin(lon1);
    double z1 = sin(lat1);

    double x2 = cos(lat2) * cos(lon2);
    double y2 = cos(lat2) * sin(lon2);
    double z2 = sin(lat2);

    double x = (x1 + x2) / 2.0;
    double y = (y1 + y2) / 2.0;
    double z = (z1 + z2) / 2.0;

    double lon = atan2(y, x);
    double hyp = sqrt(x * x + y * y);
    double lat = atan2(z, hyp);

    Position mid;
    mid.lat = toDegrees(lat);
    mid.lon = toDegrees(lon);
    return mid;
}


Street* find_closest_street(Position* posicio_user, StreetNode* head){
    StreetNode* current = head;
    Street* closest = (Street*)malloc(sizeof(Street));
    double min = 999999;
    while(current != NULL){
        Position mid = midpoint(current->carrer.from_position , current->carrer.to_position);
        double distance = haversine(*posicio_user,mid);
        if (distance < min){
            min = distance;
            *closest = current->carrer;
        }
        current = current->next;
    }
    return closest;
}

//********** CERCA LINEAL DE CONNECTED STREET **********

StreetNode* find_connected_streets(Street* current_street, StreetNode* head) {
    StreetNode* current = head;
    StreetNode* connected = NULL;

    while (current != NULL) {
        if (current->carrer.from_id == current_street->to_id) {

            connected = add_street(connected,
                                   current->carrer.from_id,
                                   current->carrer.from_position.lat,
                                   current->carrer.from_position.lon,
                                   current->carrer.to_id,
                                   current->carrer.to_position.lat,
                                   current->carrer.to_position.lon,
                                   current->carrer.lenght,
                                   current->carrer.street_name);
        }
        current = current->next;
    }
    return connected;
}

//********** CERCA PER HASH MAP DE CLOSEST STREET **********
//________HELPER FUNCTION 1________
// busquem si una intersecció ja existeix dins el hashmap.
int find_intersection_index(Hash_map* mapa, long long intersection_id){
    // recorrem la llista buscant si hi ha una intersection_id igual a la que ens dona l'usuari, en cas de ser aixi, retornem l'index d'aquest 
    for (int i = 0; i < mapa->count; i++){
        if (mapa->entries[i].intersection_id == intersection_id) return i;
    }
    // si arribem al final i no el trobem, retornem -1
    return -1;
}

//________HELPER FUNCTION 2________
// afegim un carrer a una intersecció (o creem la intersecció si no existeix).
void add_street_to_intersection(Hash_map* mapa, long long intersection_id, Street street){
    // mirem si la intersecció està al hash map
    int index = find_intersection_index(mapa, intersection_id);
    
    // si no existeix la creem
    if (index == -1){
        // comprovem que el hash
        if (mapa->count >= mapa->capacity){
            // en cas que efectivament el hash map estigui ple, multipliquem per dos la seva capacitat
            mapa->capacity *= 2;
            mapa->entries = (hash_map_entry*)realloc(mapa->entries, mapa->capacity*(sizeof(hash_map_entry)));
        }
        // creem la nova entry buida a la posició actual
        index = mapa->count;
        mapa->entries[index].intersection_id = intersection_id;
        mapa->entries[index].list_of_streets = NULL;
        mapa->count ++;
    }
    // creem un nou street list amb elnou carrer a la primera posició
    StreetNode* new_node = (StreetNode*)malloc(sizeof(StreetNode));
    new_node->carrer = street;
    new_node->next = mapa->entries[index].list_of_streets;
    mapa->entries[index].list_of_streets = new_node;
}

//\\\\\\\\\\FUNCIONS ESSENCIALS//////////

Hash_map* create_hashmap (int capacitat_inicial){    
    // creem l'estructura hash map                       
    Hash_map* hash_map = (Hash_map*)malloc(sizeof(Hash_map));
    // creem l'array del hash map
    hash_map->entries = (hash_map_entry*)malloc(capacitat_inicial * (sizeof(hash_map_entry)));

    //assignem els valors del hash_map
    hash_map->count = 0;
    hash_map->capacity = capacitat_inicial;

    // i el retornem
    return hash_map;
}

// omplir hash map de streets a paartir d'una llista d'streets
Hash_map* fill_hashmap_from_streets (StreetNode* streets_head, int initial_capacity){
    // creem un hash map buit del tamany posat per l'usuari
    Hash_map* mapa = create_hashmap(initial_capacity);

    StreetNode* current = streets_head;
    while (current != NULL){
        // mentre current current no sigui null, cridem la helper function 2 per afegir el carrer a la intersecció
        //solament cal afegir el carrer des d'on surt i al que va
        add_street_to_intersection(mapa, current->carrer.from_id, current->carrer);
        
        // un cop afegit, passem al següent
        current = current->next;
    }
    return mapa;
}

// trobar les streets connectades a una intersecció (funció important)
StreetNode* get_streets_at_intersection(Hash_map* mapa, long long intersection_id){
    // busquem si existeix la itersecció, i en cas de que si, a quin index està
    int index = find_intersection_index(mapa, intersection_id);
    // si no existeix retornem NULL
    if (index == -1) return NULL;
    // en canvi si sí que existeix, retornem la llista de carrers corresponent a l'intersecció
    return mapa->entries[index].list_of_streets;
}

// finalment alliberem la memoria guardad pel hash map
void free_hashmap(Hash_map* mapa){
    // si el mapa ja es buit no fem res
    if (mapa == NULL) return;
    // per cada entry (desde la 0 fins la numero count (fa tantes iteracions com entrades hi hagi, no més))
    for (int i = 0; i < mapa->count; i++){
        // creem un punter "current" que apunti al primer de la llista de carrers de la entry
        StreetNode* current = mapa->entries[i].list_of_streets;
        // mentre hi hagi carrers
        while (current != NULL){
            // guardem l'actual a un temporal
            StreetNode* temp = current;
            // passem al següent
            current = current->next;
            // i alliberem el temporal
            free(temp);
        }
    }
    // alliberem l'array d'entries (el "(hash_map_entry*)malloc(capacitat_inicial * (sizeof(hash_map_entry)))")
    free(mapa->entries);
    // i alliberem l'estructura del hash map
    free(mapa);
}

void latlon_to_xy(double lat_ref, double lon_ref,
                  double lat, double lon,
                  double *x, double *y) {
    double lat_ref_rad = toRadians(lat_ref);
    double dlat = toRadians(lat - lat_ref);
    double dlon = toRadians(lon - lon_ref);
    *x = EARTH_RADIUS * dlon * cos(lat_ref_rad);
    *y = EARTH_RADIUS * dlat;
}

char* calculate_turn(Position A, Position B, Position C) {
    double ax, ay, bx, by, cx, cy;

    // passem les coordenades a valors 2D x i y
    latlon_to_xy(A.lat, A.lon, A.lat, A.lon, &ax, &ay);
    latlon_to_xy(A.lat, A.lon, B.lat, B.lon, &bx, &by);
    latlon_to_xy(A.lat, A.lon, C.lat, C.lon, &cx, &cy);

    // apliquem producte vectorial (Cross Product)
    double cross_product = (bx - ax) * (cy - by) - (by - ay) * (cx - bx);

    if (cross_product > 0.0) return "left";          // gir a l'esquerra
    else if (cross_product < 0.0)  return "right";   // gir a la dreta
    else return "straight";                          // seguir recte
}