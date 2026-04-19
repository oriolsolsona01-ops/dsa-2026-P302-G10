#include "sample_lib.h"
#include "houses.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

void createaleak() {
  char *foo = malloc(20 * sizeof(char));
  printf("Allocated leaking string: %s", foo);
}

int main() {

  printf("*****************\nWelcome to DSA!\n*****************\n");
  
  
  char mapa [50];
  printf("Introdueix un nom de mapa (xs_1, xs_2, md_1, lg_1, xl_1 or 2xl_1): ");
  scanf("%s", mapa);


  FILE* map_file = ask_map();
  if (map_file == NULL) return 1;

  HouseNode* list_of_houses = fill_linkedlist(map_file);
  fclose(map_file); 

  input_originposition(list_of_houses);

  return 0;
}