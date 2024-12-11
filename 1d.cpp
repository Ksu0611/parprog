#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

const int ISIZE = 5000;
const int JSIZE = 5000;

int main(int argc, char **argv)
{
    remove("1d.txt");
    int rank = 0;
    int size = 0;
    int i, j;
    double **a = (double **) calloc(ISIZE, sizeof(double **));
    for (int i = 0; i < ISIZE; ++i)
    {
        a[i] = (double *) calloc(JSIZE, sizeof(double *));
    }

    for (i = 0; i < ISIZE; i++)
    {
        for (j = 0; j < JSIZE; j++)
        {
            a[i][j] = 10 * i + j;
        }
    }
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;
    
    
    FILE *ff;
    int* nb;
    nb = (int *) calloc(JSIZE, sizeof(int));
    int nb1 = 0, nb2 = 0;
    

    
    nb1 = ISIZE / size; /* количество итераций на каждый поток */
    nb2 = ISIZE % size; /* количество итераций, не попавших ни в один поток */
    
    for (i = 0; i < size; i++)
    {
        nb[i] = nb1;
    }
    nb[size - 1] = nb[size - 1] + nb2 - 6;
    double start = MPI_Wtime();
    for (i = 0; i < ISIZE - 1; i++)
    {
        for (j = 6; j < nb[rank] + 6; j++)
        {
            a[i][j + rank * nb1] = sin(0.2 * a[i + 1][j + rank * nb1 - 6]);
            //a[i * 5000 + j1] = a[(i + 1)* 5000 + j2];
        }
    }
    //printf("%f\n", a[1400][1400]);
    MPI_Barrier(MPI_COMM_WORLD);
    double stop = MPI_Wtime();
    printf("Time spent: %lf sec\n", (stop - start));
    ff = fopen("1d.txt", "a");
    if (rank == 0)
    {
         //printf("%d\n\n\n\n", nb[rank]);
        for (i = 0; i < ISIZE; i++)
        {
            for (j = 0; j < nb[rank]; j++)
            {
                fprintf(ff, "%f ", a[i][j + rank * nb1]);
            }
            fprintf(ff, "\n");
        }
        MPI_Send(&stop, 1, MPI_DOUBLE, rank + 1, size + 1, MPI_COMM_WORLD);
    }
    else
    {
       // printf("%d\n", rank);
        if (rank == size - 1)
        {
            MPI_Recv(&stop, 1, MPI_DOUBLE, rank - 1, size + 1, MPI_COMM_WORLD, &status);
            for (i = 0; i < ISIZE; i++)
            {
                for (j = 0; j < nb[rank]; j++)
                {
                    fprintf(ff, "%f ", a[i][j + rank * nb1]);
                }
                fprintf(ff, "\n");
            }
        }
        else
        {
            MPI_Recv(&stop, 1, MPI_DOUBLE, rank - 1, size + 1, MPI_COMM_WORLD, &status);
            for (i = 0; i < ISIZE; i++)
            {
                for (j = 0; j < nb[rank]; j++)
                {
                    fprintf(ff, "%f ", a[i][j + rank * nb1]);
                }
                fprintf(ff, "\n");
            }
            MPI_Send(&stop, 1, MPI_DOUBLE, rank + 1, size + 1, MPI_COMM_WORLD);
        }
       
    }
    fclose(ff);

    MPI_Finalize();
    return 0;
}
