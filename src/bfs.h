#include "streets.h"
#define MAX_PATH_LEN 200
#define QUEUE_SIZE   2000

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