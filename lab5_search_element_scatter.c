#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int x, x_receve, count;
    int n = 10;
    int *buffer, *buffer_receve;
// Input reading for process 0

    if (rank == 0) {
        printf("Digite o elemento x\n");
        scanf("%d", &x);
        buffer = (int *) malloc(sizeof(int) * n);
        for (int i = 0; i < n; ++i) {
            buffer[i] = (int) rand() % 10 + 1;
            printf("Elemento %d do processo %d\n", buffer[i], rank);
        }
    }
    buffer_receve = (int *) malloc(sizeof(int) * 2);
    MPI_Bcast(&x, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Scatter(buffer, 2, MPI_INT, buffer_receve, 2, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0) {
        for (int i = 0; i < 2; ++i) {
            if (x == buffer_receve[i]) {
                printf("O elemento %d do rank %d pertence ao conjunto S\n", buffer_receve[i], rank);
            }
        }
    }

    MPI_Finalize();

    return 0;
}