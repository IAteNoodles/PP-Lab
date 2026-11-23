// Program 7: OpenMP static scheduling, chunk size 2
#include <stdio.h>
#include <omp.h>

int main(void) {
    int n, first[100], last[100];
    printf("Enter number of iterations: ");
    scanf("%d", &n);

    for (int i = 0; i < 100; i++) first[i] = last[i] = -1;

    #pragma omp parallel for schedule(static,2)
    for (int i = 0; i < n; i++) {
        int tid = omp_get_thread_num();
        if (first[tid] == -1) first[tid] = i;
        last[tid] = i;
    }

    for (int i = 0; i < 100; i++)
        if (first[i] != -1)
            printf("Thread %d : Iterations %d -- %d\n", i, first[i], last[i]);

    return 0;
}
