#include "houses.h"

HouseNode* add_house(HouseNode* head, char* street, int number, double lat, double lon) {
    HouseNode* new_house = (HouseNode*)malloc(sizeof(HouseNode));
    
    strcpy(new_house->street_name,street);
    new_house->house_number = number;
    new_house->lat = lat;
    new_house->lon = lon;

    new_house->next = head;

    return new_house;
}