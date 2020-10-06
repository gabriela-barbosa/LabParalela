#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define buffer_count 5

int main(int argc, char **argv)
{
    // initialization
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    float buffer[buffer_count];
    memset(buffer, 0, sizeof(buffer));

    float buffer_without_reduce[buffer_count];
    memset(buffer_without_reduce, 0, sizeof(buffer_without_reduce));

    // Uniform sampling, generating the numbers and doing 1000 repetitions
    for (int rep = 0; rep < 1000; ++rep)
    {
        for (int i = 0; i < buffer_count; ++i)
        {
            float val = (float)rand() / RAND_MAX;

            buffer[i] += val;
        }
    }
    //Implementação sem MPI_Reduce

    //Copiando valores random do buffer para o buffer que será utilizado para a implementação sem o reduce

    for (int p = 0; p < buffer_count; ++p)
    {
        buffer_without_reduce[p] = buffer[p];
    }

    //Implementação do reduce
    //Rank 0 recebe todos os resultados dos buffers dos outros ranks
    if (rank == 0)
    {
        for (int i = 0; i < buffer_count; ++i)
        {
            //Para cada processo menos o de rank 0
            for (int p = 0; p < size - 1; ++p)
            {
                float valor;
                MPI_Recv(&valor, 1, MPI_FLOAT, p + 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                buffer_without_reduce[i] += valor;
            }
        }
    }
    else
    {
        for (int p = 0; p < buffer_count; ++p)
        {
            MPI_Send(&buffer[p], 1, MPI_FLOAT, 0, 1, MPI_COMM_WORLD);
        }
    }


    float reception[buffer_count];
    memset(reception, 0, sizeof(reception));
    MPI_Reduce(&buffer, &reception, buffer_count, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Now we print the results
    if (rank == 0)
    {
        printf("------------MPI_Reduce------------\n");
        for (int i = 0; i < buffer_count; ++i)
        {
            printf("  reception[%d] %f\n", i, reception[i]);
        }
        printf("----------Sem MPI_Reduce----------\n");
        for (int p = 0; p < buffer_count; ++p)
        {
            printf("  reception[%d] %f\n", p, buffer_without_reduce[p]);
        }
    }

    MPI_Finalize();

    return 0;
}