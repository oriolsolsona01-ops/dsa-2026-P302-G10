#include <stdio.h>
#include <string.h>

int minimum (int a, int b, int c) {
    int min = a;
    if (b < min) min = b;
    if (c < min) min = c;
    return min;
}

int LevenshteinDistance (char *a, char *b){
    int m = len(a);
    int n = len(b);

    int **D = (int**)malloc(((m+1) * sizeof(int*)));
    for (int i = 0; i <= m; i++){
        D[i] = (int*)malloc((n+1)*sizeof(int));
    }

    for (int i = 0; i <= m; i++){
        D[i][0] = i;
    }

    for (int j = 0; j <= n; j++){
        D[0][j] = j;
    }

    for (int i = 1; i < m; i++){
        for (int j = 1; j < n; j++){
            int cost;
            if (a[i - 1] == b[j - 1]) cost = 0;
            
            else cost = 1;

            D[i][j] = minimum(D[i-1][j] + 1, D[i][j-1] + 1, D[i-1][j-1] + cost);
        }
    }
    int result = D[m][n];

    for (int i = 0; i <= m; i++){
        free(D[i]);
    }
    free(D);

    return result;
}
