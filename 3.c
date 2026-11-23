// Program 3: MPI_Bcast
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, n;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        printf("What is the input given in the present case: ");
        scanf("%d", &n);
        printf("Process %d broadcasting value %d to all other processes\n", rank, n);
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Process %d received value %d\n", rank, n);

    MPI_Finalize();
    return 0;
}
