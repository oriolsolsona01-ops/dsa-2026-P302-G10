#include "sample_lib.h"
#include "house.h"
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

    void houselinked_list (FILE *fitxer){
        while (!eof) {
            add_house()
            fscanf()


        }











    }
// -- ORIGIN

void input_originposition(FILE* fitxer){ // tasca 2,3 i 4 (el paràmtre d'entrada serà el punter al doc)

    int posicio_origen;
    int num;
    char street_name[100];
    scanf("Com voleu introduir la vostra posició d'origen? Adreça (1), Lloc (2) or Coordenada (3) %d", &posicio_origen);
    while (posicio_origen < 1 || posicio_origen > 3){
        scanf("Com voleu introduir la vostra posició d'origen? Adreça (1), Lloc (2) or Coordenada (3) %d", &posicio_origen);

    }
    if (posicio_origen == 2 || posicio_origen == 3){

        printf("Encara no hem implementat aquesta funcionalitat. \n");

    }
    
    else{

        scanf("Introdueix el nom del carrer %s", street_name);
        scanf("Introdueix el número del carrer %d", num);
        //cal trobar les coordenades un cop tenint aquestes dades amb el nom i num amb fscanf
        char* coordenades;
    }

    
    
}





