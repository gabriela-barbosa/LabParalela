#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int x, x_receive, element, num;
    int n = 10;

    if (rank == 0) {
        printf("Digite o elemento x\n");
        scanf("%d", &x);
        for (int i = 1; i < n; ++i) {
            num = (int) rand() % 10 + 1;
            printf("Elemento %d \n", num);
            MPI_Send(&x, 1, MPI_INT, i % size, 0, MPI_COMM_WORLD);
            MPI_Send(&num, 1, MPI_INT, i % size, 0, MPI_COMM_WORLD);
        }
    } else {
        for (int i = 0; i < (rank / size) + 1; ++i) {
            MPI_Recv(&x_receive, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&element, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("passei %d pertence ao conjunto %d rankS  %d\n", element, rank, (rank / size) + 1);

            if (x_receive == element) {
                printf("O número %d pertence ao conjunto S\n", x_receive);
            }
        }
    }
    MPI_Finalize();

    return 0;
}