#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int minimum (int a, int b, int c) {
    int min = a;
    if (b < min) min = b;
    if (c < min) min = c;
    return min;
}

int LevenshteinDistance (char *a, char *b){
    int m = strlen(a);
    int n = strlen(b);

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

    for (int i = 1; i <= m; i++){
        for (int j = 1; j <= n; j++){
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

int main(){

    char *a = "CALLE";
    char *b = "VALLE";
    char *c = "VALLe";

    int a_1 = LevenshteinDistance(a,b);
    printf("canvis entre CALLE i VALLE = %d\n", a_1);

    int a_2 = LevenshteinDistance(b,c);
    printf("canvis entre VALLE i VALLe = %d\n", a_2);

    int a_3 = LevenshteinDistance(a,c);
    printf("canvis entre CALLE i VALLe = %d\n", a_3);
}