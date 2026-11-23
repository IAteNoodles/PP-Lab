// Program 9: Prime numbers using parallel for
#include <stdio.h>
#include <omp.h>

int is_prime(int n) {
    if (n < 2) return 0;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0) return 0;
    return 1;
}

int main(void) {
    int n, count = 0;
    volatile int sink;
    printf("Enter the value of n: ");
    scanf("%d", &n);

    printf("\nPrime numbers from 1 to %d:\n", n);
    for (int i = 1; i <= n; i++)
        if (is_prime(i)) printf("%d ", i);
    printf("\n");

    double start = omp_get_wtime();
    for (int i = 1; i <= n; i++) count += is_prime(i);
    sink = count;
    printf("Serial Time: %f seconds\n", omp_get_wtime() - start);

    count = 0;
    start = omp_get_wtime();
    #pragma omp parallel for reduction(+:count)
    for (int i = 1; i <= n; i++) count += is_prime(i);
    sink = count;
    printf("Parallel Time: %f seconds\n", omp_get_wtime() - start);

    return 0;
}
