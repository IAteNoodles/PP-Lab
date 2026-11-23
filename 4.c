// Program 4: MPI_Scatter and MPI_Gather
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size, send[100], recv, gathered[100];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
        for (int i = 0; i < size; i++) send[i] = i * 10;

    MPI_Scatter(send, 1, MPI_INT, &recv, 1, MPI_INT, 0, MPI_COMM_WORLD);
    recv += rank;
    MPI_Gather(&recv, 1, MPI_INT, gathered, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Gathered data in root process:\n");
        for (int i = 0; i < size; i++)
            printf("gathered_data[%d] = %d\n", i, gathered[i]);
    }

    MPI_Finalize();
    return 0;
}
