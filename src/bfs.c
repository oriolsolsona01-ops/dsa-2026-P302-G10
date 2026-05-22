#include "sample_lib.h"
#include "streets.h"
#include "places.h"
#include "houses.h"
#include "bfs.h"
#include <stdio.h>
#include <stdlib.h>


void queue_init(Queue* q) {
    q->head = 0;
    q->tail  = 0;
    q->size  = 0;
}

int queue_is_empty(Queue* q) {
    return q->size == 0;
}

void enqueue(Queue* q, Path p) {
    if (q->size >= QUEUE_SIZE) return;   // cua plena, no fem res
    q->items[q->tail] = p;
    q->tail  = (q->tail + 1) % QUEUE_SIZE;
    q->size++;
}

Path dequeue(Queue* q) {
    Path p   = q->items[q->head];
    q->head = (q->head + 1) % QUEUE_SIZE;
    q->size--;
    return p;
}

// compara si dos carrers són el mateix segment

int same_street(Street* a, Street* b) {
    return (a->from_id == b->from_id && a->to_id == b->to_id);
}

// comprova si un carrer ja és al array de visitats 

int is_visited(Street* visited, int count, Street* s) {
    for (int i = 0; i < count; i++) {
        if (same_street(&visited[i], s)) return 1;
    }
    return 0;
}

Path* BFS(Hash_map* hash_map, Street* from_street, Street* to_street) {

    Queue Q;
    queue_init(&Q);

    // visited: array de carrers ja explorats
    Street visited[MAX_PATH_LEN * 4];
    int visited_count = 0;

    // camí inicial: només conté el carrer d'origen
    Path initial_path;
    initial_path.streets[0] = *from_street;
    initial_path.length = 1;
    enqueue(&Q, initial_path);

    while (!queue_is_empty(&Q)) {

        Path path = dequeue(&Q);
        Street current_street = path.streets[path.length - 1];  // path[-1]

        // si arribem al ultim carrer
        if (same_street(&current_street, to_street)) {
            Path* result = (Path*)malloc(sizeof(Path));
            *result = path;
            return result;
        }

        if (!is_visited(visited, visited_count, &current_street)) {

            // marquem com a visitat
            visited[visited_count] = current_street;
            visited_count++;

            // mirem tots els carrers connectats a la intersecció final
            // (intersections_graph[current_street.to_intersection_id])
            StreetNode* connected = get_streets_at_intersection(hash_map, current_street.to_id);
            while (connected != NULL) {
                Street* neighbor = &connected->carrer;

                if (!is_visited(visited, visited_count, neighbor)) {
                    // new_path = path + [connected_street]
                    Path new_path = path;
                    new_path.streets[new_path.length] = *neighbor;
                    new_path.length++;
                    enqueue(&Q, new_path);
                }
                connected = connected->next;
            }
        }
    }

    return NULL;  // no s'ha trobat cap camí
}