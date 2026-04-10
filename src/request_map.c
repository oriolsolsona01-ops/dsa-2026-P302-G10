#include <stdio.h>
#include <string.h>

int cost (char c, char d){
    for (int i = 1; i < m; i++){
        for (int j = 1; j < n; j++){
            if (a[i-1] == b[j-1]){
                int cost = 0;
            } else {
                int cost = 1;
            }
        }
    }
}

int LevenshteinDistance (char a, char b){
    int m = len(a);
    int n = len(b);

    int **D = (int)malloc(((m+1) * sizeof(int)));
    for (int i = 0; i <= m; i++){
        D[i] = (int)malloc((n+1)*sizeof(int));
    }

    for (int i = 0; i <= m; i++){
        D[i] = i;
    }

    for (int j = 0; j <= n; j++){
        D[0][j] = j;
    }

    int cost = cost(a,b);

            D[i][j] ← minimum(
                D[i-1][j] + 1,      // deletion
                D[i][j-1] + 1,      // insertion
                D[i-1][j-1] + cost  // substitution
            )

    return min;
}
char 