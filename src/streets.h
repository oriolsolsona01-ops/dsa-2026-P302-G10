#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#define MAX_PATH_LEN 200
#define QUEUE_SIZE   2000

typedef struct Position {
  double lat;
  double lon;
} Position;

// estructura d'un carrer
typedef struct Street{
    // dades de la posició inicial del carrer
    int from_id;                // from_intersaction_id, 
    Position from_position;
    
    // dades de la posició final del carrer
    int to_id;                  // to_intersection_id, 
    Position to_position;       

    // dades generals del carrer
    double lenght;
    char street_name[100];
} Street;

// estructura d'un node d'un carrer
typedef struct StreetNode{
    struct Street carrer;
    struct StreetNode* next;
} StreetNode;

// ********** ESTRUCTURES PEL HASH MAP **********
typedef struct hash_map_entry{
    int intersection_id;             // key
    StreetNode* list_of_streets;     // value
} hash_map_entry;

typedef struct Hash_map{
    hash_map_entry* entries;          // array of entries
    int count;                       // intersections counter
    int capacity;                    // total array capacity
} Hash_map;


FILE* open_map_streets(char* mapa);
StreetNode* add_street(StreetNode* head, int from, double from_lat, double from_lon, int to, double to_lat, double to_lon, double len, char* name);
StreetNode* fill_linked_streets(FILE *fitxer);
double toRadians(double degree);
double toDegrees(double radians);
double haversine(Position posA, Position posB);
Position midpoint(Position a, Position b);
Street* find_closest_street(Position* posicio_user, StreetNode* head);
StreetNode* find_connected_streets(Street* current_street, StreetNode* head);
// funcions del hash_map
Hash_map* create_hashmap (int capacitat_inicial);
Hash_map* fill_hashmap_from_streets (StreetNode* streets_head, int initial_capacity);
StreetNode* get_streets_at_intersection(Hash_map* map, int intersection_id);
void free_hashmap(Hash_map* map);

// bfs
// Un camí és un array de Streets + la seva longitud
typedef struct {
    Street streets[MAX_PATH_LEN];
    int length;
} Path;

// La cua guarda Paths
typedef struct {
    Path  items[QUEUE_SIZE];
    int   head, tail, size;
} Queue;

Path* BFS(Hash_map* hash_map, Street* from_street, Street* to_street);