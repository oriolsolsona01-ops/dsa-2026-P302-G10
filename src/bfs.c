#include "sample_lib.h"
#include "streets.h"
#include "places.h"
#include "houses.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
/*
BFS(intersections_graph, fromStreet, toStreet):
    create an empty queue of street lists, Q

    create a street list [fromStreet], initial_path
    enqueue initial_path into Q
    create a street list [], visited

    while Q is not empty:
        path = dequeue(Q)
        current_street = path[-1]

        if current_street == toStreet:
            return path
        
        if current_street not in visited:
            add current_street to visited

            for connected_street in intersections_graph[current_street.to_intersection_id]:
                if connected_street not in visited:
                    new_path = path + [connected_street]
                    enqueue new_path into Q

    return NULL   # no path found
*/
BFS( intersections_graph,Street* fromStreet,Street* toStreet){
    
}