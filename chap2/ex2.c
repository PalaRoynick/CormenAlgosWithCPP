#include "stdio.h"
#include "stdlib.h"

void merge(int *A, int p, int q, int r) {
    printf("*begin = %d, *mid = %d, *end = %d\n", A[p], A[q], A[r]);s
    int n1, n2, i, j, k;
    n1 = q - p + 1;
    n2 = r - q;
    int *L = malloc(n1);
    int *R = malloc(n2);

    for (i = 0; i < n1; ++i)
        L[i] = A[p + i];
    for (j = 0; j < n2; ++j)
        R[j] = A[q + j + 1];

    i = 0; j = 0;
    k = p;

    while ((i < n1) && (j < n2)) {
        if (L[i] <= R[j])
            A[k++] = L[i++];
        else
            A[k++] = R[j++];
    }
    while (i < n1) {
        A[k++] = L[i++];
    }
    while (j < n2) {
        A[k++] = R[j++];
    }

    free(L); free(R);
}

void merge_sort(int *A, int p, int r) {
    if (p < r) {
        int q = (p + r) / 2;
        merge_sort(A, p, q);
        merge_sort(A, q + 1, r);
        merge(A, p, q, r);
    }
}

int main() {
    int A[] = { 5,2,4,7,1,3,2,6 };
    merge_sort(A, 0, 7);
    for (int i = 0; i < 8; ++i) {
        printf("%d, ", A[i]);
    }
}