#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int n_elements;
    double *buffer;
    // Input reading for process 0

    if (rank == 0) {
        /********* reading the number of elements in n_elements  ********/
        printf("Digite o número de elementos\n");
        scanf("%d", &n_elements);
        buffer = (double *) malloc(sizeof(double) * n_elements);
        double soma = 0;
        for (int i = 0; i < n_elements; ++i) {
            double element;
            printf("Digite o elemento %d\n", i);
            scanf("%lf", &element);
            buffer[i] = element;
            soma += buffer[i];
            printf("Elemento %f do processo %d\n", buffer[i], rank);
        }
        printf("Soma do processo %d é igual a %f\n", rank, soma);

        MPI_Bcast(&n_elements, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(buffer, n_elements, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        free(buffer);
//
    }

    if (rank != 0) {
        double *buffer1, *buffer2;
        int elements;
        MPI_Bcast(&elements, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (rank == 1) {
            double somaPositive = 0;
            buffer1 = (double *) malloc(sizeof(double) * elements);
            MPI_Bcast(buffer1, elements, MPI_DOUBLE, 0, MPI_COMM_WORLD);
            for (int i = 0; i < n_elements; ++i) {
                if (buffer1[i] > 0) {
                    printf("Elemento %f do processo %d\n", buffer1[i], rank);
                    somaPositive += buffer1[i];
                }
            }
            printf("Soma do processo %d é igual a %f\n", rank, somaPositive);
            free(buffer1);
        }
        if (rank == 2) {
            double somaNegative = 0;
            buffer2 = (double *) malloc(sizeof(double) * elements);
            MPI_Bcast(buffer2, elements, MPI_DOUBLE, 0, MPI_COMM_WORLD);
            for (int i = 0; i < n_elements; ++i) {
                if (buffer2[i] < 0) {
                    printf("Elemento %f do processo %d\n", buffer2[i], rank);
                    somaNegative += buffer2[i];
                }
            }
            printf("Soma do processo %d é igual a %f\n", rank, somaNegative);
            free(buffer2);
        }
    }
    MPI_Finalize();

    return 0;
}
