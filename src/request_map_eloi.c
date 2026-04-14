#include "sample_lib.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>


void find_street(char mapa[], char street[]){
    mapa = strcat("maps/",mapa);
    mapa = strcat(mapa,"/houses.txt");
    FILE* f_mapa = fopen(mapa,"r");
        if (f_mapa == NULL){
            printf("[ERROR] File does not open\n");
            return -1;
        }

    while (!EOF){
        fscanf(f_mapa,"");
        LevenshteinDistance(street,)
    }

}

    