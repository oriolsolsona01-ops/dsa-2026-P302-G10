#include "sample_lib.h"
#include "streets.h"
#include "places.h"
#include "houses.h"
#include <stdio.h>
#include <stdlib.h>




// compara si dos carrers són el mateix segment
int same_street(Street* a, Street* b) {
    return (a->from_id == b->from_id && a->to_id == b->to_id);
}

// Retorna la llista enllaçada del camí (inici fins a fi)
StreetNode* BFS(Hash_map* intersections_graph, Street* fromStreet, Street* toStreet) {
    
    int max_size = 300000; 
    Street* cua_carrers = (Street*)malloc(max_size * sizeof(Street));
    int* cua_pares = (int*)malloc(max_size * sizeof(int));
    
    Hash_map* visited = create_hashmap(1024);

    int head = 0;
    int tail = 0;

    // encoem inicialment
    cua_carrers[tail] = *fromStreet;
    cua_pares[tail] = -1; // -1 vol dir que som a l'origen, no tenim pare
    add_street_to_intersection(visited, fromStreet->from_id, *fromStreet);
    tail++;

    int found_index = -1; // guardarà la posició on hem trobat el destí

    while (head < tail) {
        int current_index = head;
        Street current_street = cua_carrers[head];
        head++;

        // Hem arribat al final?
        if (same_street(&current_street, toStreet)) {
            found_index = current_index;
            break;
        }

        // busquem cap a on podem anar
        StreetNode* connected = get_streets_at_intersection(intersections_graph, current_street.to_id);
        // la funció get streets at inters. el que fa és el punter amb els carrers veïns per on el BFS pot continuar.
        
        while (connected != NULL) {
            Street* neighbor = &connected->carrer;

            // buscar si el aquest vei ja ha estat visitat (aixo es cerca lineal, podem utilitzar hash_map per guardar els visited)
            int ja_visitat = 0;
            StreetNode* v = get_streets_at_intersection(visited, neighbor->from_id);
            while (v != NULL) {
                if (v->carrer.to_id == neighbor->to_id) { ja_visitat = 1; break; }
                v = v->next;
            }

            if (!ja_visitat && tail < max_size) {
                cua_carrers[tail] = *neighbor;
                cua_pares[tail] = current_index; // el parent pointerç
                add_street_to_intersection(visited, neighbor->from_id, *neighbor);
                tail++;
            }
            connected = connected->next;
        }
    }

    StreetNode* path_head = NULL;

    // reconstruïm el camí saltant de pare a pare fins arribar a l'origen
    if (found_index != -1) {
        int curr = found_index;
        while (curr != -1) {
            Street c = cua_carrers[curr];
            // afegim al cap de la llista, així al final quedarà ordenat
            path_head = add_street(path_head, c.from_id, c.from_position.lat, c.from_position.lon,
                                   c.to_id, c.to_position.lat, c.to_position.lon, c.lenght, c.street_name);
            
            curr = cua_pares[curr]; // retrocedim 1 salt
        }
    }
    free_hashmap(visited);
    free(cua_carrers);
    free(cua_pares);
    
    return path_head;
}