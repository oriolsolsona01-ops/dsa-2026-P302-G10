#include "streets.h"
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

StreetNode* add_street(StreetNode* head, int from, double from_lat, double from_lon, 
                       int to, double to_lat, double to_lon, double len, char* name) {

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
    int from, to;
    double from_lat, from_lon, to_lat, to_lon, len;
    char name[100];
    StreetNode* streets = NULL;

    while (fscanf(fitxer,"%[^,], %d,%lf,%lf,%d,%lf,%lf,%lf", 
           &from, &from_lat, &from_lon, &to, &to_lat, &to_lon, &len, name) == 8) {

        // Cridem a add_street passant-li el "cap" actual (streets)
        streets = add_street(streets, from, from_lat, from_lon, to, to_lat, to_lon, len, name); 
    }

    return streets; 
}



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
    Street* closest = NULL;
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

StreetNode* find_connected_streets(Street* current_street, StreetNode* head) {
    StreetNode* current = head;
    StreetNode* connected = NULL;

    while (current != NULL) {
        if ((current->carrer.from_id == current_street->from_id ||
             current->carrer.from_id == current_street->to_id)  ||
            (current->carrer.to_id   == current_street->from_id ||
             current->carrer.to_id   == current_street->to_id)) {

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
