#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int x, x_receve;
    int n = 10;
    int *buffer, *buffer_receve;
// Input reading for process 0

    if (rank == 0) {
/********* reading the number of elements in n_elements  ********/
        printf("Digite o elemento x\n");
        scanf("%d", &x);
        buffer = (int *) malloc(sizeof(int) * n);
        for (int i = 0; i < n; ++i) {
            buffer[i] = (int) rand() % 10 + 1;
            printf("Elemento %d do processo %d\n", buffer[i], rank);
        }
        for (int i = 1; i < size; ++i) {
            MPI_Send(&x, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(buffer, n, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        free(buffer);
    } else {
        buffer_receve = (int *) malloc(sizeof(int) * n);
        MPI_Recv(&x_receve, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(buffer_receve, n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int bool = 0;
        for (int i = 0; i < n; ++i) {
            if (x_receve == buffer_receve[i]) {
                bool = 1;
            }
        }
        if (bool) {
            printf("O número %d pertence ao conjunto S\n", x_receve);
        } else {
            printf("O número %d não pertence ao conjunto S\n", x_receve);
        }
    }

    MPI_Finalize();

    return 0;
}