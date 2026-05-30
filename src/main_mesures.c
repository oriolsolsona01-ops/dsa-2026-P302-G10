#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "sample_lib.h"
#include "streets.h"
#include "places.h"
#include "houses.h"

/* BFS original de bfs.c - no es toca */
StreetNode* BFS(Hash_map* intersections_graph, Street* fromStreet, Street* toStreet);

/* BFS amb capacitat configurable de bfs_mesures.c */
StreetNode* BFS_cap(Hash_map* intersections_graph, Street* fromStreet, Street* toStreet, int visited_capacity);

static double ms(clock_t start, clock_t end) {
    return ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;
}

static StreetNode* load_streets(const char* path) {
    FILE* f = fopen(path, "r");
    if (f == NULL) { printf("  [!] No trobat: %s\n", path); return NULL; }
    StreetNode* streets = fill_linked_streets(f);
    fclose(f);
    return streets;
}

/* =====================================================================
   TAULA 2.1
   Mesura: trobar carrers connectats a una interseccio
   - Sequencial: recorre tota la llista un per un -> O(N)
   - Hashmap:    accés directe per ID             -> O(1)
   ===================================================================== */
void taula_2_1(void) {
    printf("\n=================================================================\n");
    printf("  TAULA 2.1 - Carrers connectats: sequencial vs hashmap\n");
    printf("=================================================================\n");
    printf("%-8s | %10s | %18s | %16s\n", "Mapa", "Segments", "Sequencial (ms)", "Hashmap (ms)");
    printf("---------+------------+--------------------+------------------\n");

    const char* paths[] = {
        "maps/xs_2/streets.txt",
        "maps/md_1/streets.txt",
        "maps/lg_1/streets.txt",
        "maps/xl_1/streets.txt"
    };
    const char* noms[] = { "xs_2", "md_1", "lg_1", "xl_1" };

    for (int i = 0; i < 4; i++) {
        StreetNode* streets = load_streets(paths[i]);
        if (streets == NULL) continue;

        int count = 0;
        StreetNode* tmp = streets;
        while (tmp != NULL) { count++; tmp = tmp->next; }

        long long test_id = streets->carrer.to_id;

        /* CERCA SEQUENCIAL */
        clock_t t0 = clock();
        StreetNode* cur = streets;
        while (cur != NULL) {
            if (cur->carrer.from_id == test_id) { }
            cur = cur->next;
        }
        clock_t t1 = clock();
        double t_seq = ms(t0, t1);

        /* CERCA HASHMAP */
        Hash_map* hmap = fill_hashmap_from_streets(streets, 8192);
        clock_t t2 = clock();
        get_streets_at_intersection(hmap, test_id);
        clock_t t3 = clock();
        double t_hash = ms(t2, t3);

        printf("%-8s | %10d | %18.4f | %16.4f\n", noms[i], count, t_seq, t_hash);

        free_hashmap(hmap);
        free_linked_streets(streets);
    }
}

/* =====================================================================
   TAULA 2.2
   Mesura: BFS complet amb hashmap de veins rapid vs lent
   - BFS seq:  hashmap veins capacity=1    -> cerca veins O(N)
               hashmap visitats capacity=1 -> visitats O(N)
   - BFS hash: hashmap veins capacity=8192 -> cerca veins O(1)
               hashmap visitats capacity=1024 -> visitats O(1)
   ===================================================================== */
void taula_2_2(void) {
    printf("\n=================================================================\n");
    printf("  TAULA 2.2 - BFS complet: sequencial vs hashmap\n");
    printf("=================================================================\n");
    printf("%-8s | %10s | %18s | %16s\n", "Mapa", "Segments", "BFS seq (ms)", "BFS hash (ms)");
    printf("---------+------------+--------------------+------------------\n");

    const char* paths[] = {
        "maps/xs_2/streets.txt",
        "maps/md_1/streets.txt",
        "maps/lg_1/streets.txt",
        "maps/xl_1/streets.txt"
    };
    const char* noms[] = { "xs_2", "md_1", "lg_1", "xl_1" };

    for (int i = 0; i < 4; i++) {
        StreetNode* streets = load_streets(paths[i]);
        if (streets == NULL) continue;

        int count = 0;
        StreetNode* tmp = streets;
        while (tmp != NULL) { count++; tmp = tmp->next; }

        Street* from_s = &(streets->carrer);
        StreetNode* tail = streets;
        while (tail->next != NULL) tail = tail->next;
        Street* to_s = &(tail->carrer);

        /* BFS HASHMAP: veins O(1) + visitats O(1) */
        Hash_map* hmap = fill_hashmap_from_streets(streets, 8192);
        clock_t t0 = clock();
        StreetNode* path_hash = BFS_cap(hmap, from_s, to_s, 1024);
        clock_t t1 = clock();
        double t_bfs_hash = ms(t0, t1);
        if (path_hash) free_linked_streets(path_hash);
        free_hashmap(hmap);

        /* BFS SEQUENCIAL: veins O(N) + visitats O(N) */
        Hash_map* hmap_seq = fill_hashmap_from_streets(streets, 1);
        clock_t t2 = clock();
        StreetNode* path_seq = BFS_cap(hmap_seq, from_s, to_s, 1);
        clock_t t3 = clock();
        double t_bfs_seq = ms(t2, t3);
        if (path_seq) free_linked_streets(path_seq);
        free_hashmap(hmap_seq);

        printf("%-8s | %10d | %18.4f | %16.4f\n", noms[i], count, t_bfs_seq, t_bfs_hash);

        free_linked_streets(streets);
    }
}

/* =====================================================================
   TAULA 2.3
   Mesura: BFS vs distancia origen-desti (mapa xl_1)
   4 parelles a distancies creixents des de UPF
   ===================================================================== */
void taula_2_3(void) {
    printf("\n=================================================================\n");
    printf("  TAULA 2.3 - BFS vs distancia (mapa xl_1)\n");
    printf("=================================================================\n");
    printf("%-38s | %16s\n", "Ruta", "BFS hash (ms)");
    printf("---------------------------------------+------------------\n");

    StreetNode* streets = load_streets("maps/xl_1/streets.txt");
    if (streets == NULL) return;

    Hash_map* hmap = fill_hashmap_from_streets(streets, 8192);

    const double orig_lat = 41.403782, orig_lon = 2.193446;
    const double dest_lat[] = { 41.403565, 41.397530, 41.403530, 41.389559 };
    const double dest_lon[] = { 2.186490,  2.180820,  2.153870,  2.135112  };
    const char* labels[] = {
        "~500m   (UPF->Parc Ciutadella)",
        "~2km    (UPF->Estacio de Franca)",
        "~5km    (UPF->Glories)",
        "~10km   (UPF->L'Illa Diagonal)"
    };

    Position pos_orig = { orig_lat, orig_lon };
    Street* from_s = find_closest_street(&pos_orig, streets);

    for (int i = 0; i < 4; i++) {
        Position pos_dest = { dest_lat[i], dest_lon[i] };
        Street* to_s = find_closest_street(&pos_dest, streets);

        if (from_s == NULL || to_s == NULL) {
            printf("%-38s | %16s\n", labels[i], "N/A");
            continue;
        }

        clock_t t0 = clock();
        StreetNode* path = BFS_cap(hmap, from_s, to_s, 1024);
        clock_t t1 = clock();
        double t_hash = ms(t0, t1);
        if (path) free_linked_streets(path);

        printf("%-38s | %16.4f\n", labels[i], t_hash);

        free(to_s);
    }

    free(from_s);
    free_hashmap(hmap);
    free_linked_streets(streets);
}

int main(void) {
    printf("\n**************************************************\n");
    printf("   MESURES AUTOMATIQUES PER AL REPORT (DSA 25/26)\n");
    printf("**************************************************\n");

    taula_2_1();
    taula_2_2();
    taula_2_3();

    printf("\n--- MESURES COMPLETADES ---\n\n");
    return 0;
}