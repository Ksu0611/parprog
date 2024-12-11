 #include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define ISIZE 5000
#define JSIZE 5000

int main(int argc, char **argv)
{
    remove("basic.txt");
    int rank = 0;
    int size = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;
    int* nb;
    nb = (int *) calloc(JSIZE, sizeof(int));
    int nb1 = 0, nb2 = 0;
    
    double *a[ISIZE];
    int i, j;
    for (i = 0; i < ISIZE; i++)
    {
        a[i] = (double *)calloc((size_t)JSIZE, sizeof(double));
    }
    FILE *ff;
    for (i = 0; i < ISIZE; i++)
    {
        for (j = 0; j < JSIZE; j++)
        {
            a[i][j] = 10 * i + j;
        }
    }
    nb1 = ISIZE / size; /* количество итераций на каждый поток */
    nb2 = ISIZE % size; /* количество итераций, не попавших ни в один поток */
    
    for (i = 0; i < size; i++)
    {
        nb[i] = nb1;
    }
    nb[size - 1] = nb[size - 1] + nb2; /* последний поток будет исполняться больше итераций */
    MPI_Barrier(MPI_COMM_WORLD);
    double t_start = MPI_Wtime();
    // начало измерения времени
    for (i = 0; i < nb[rank]; i++)
    {
        for (j = 0; j < JSIZE; j++)
        {
            a[i + rank * nb1][j] = sin(2 * a[i + rank * nb1][j]);
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    double t_end = MPI_Wtime();
    double working_time = t_end - t_start;
    // окончание измерения времени
    printf("%f\n", working_time);

    ff = fopen("basic.txt", "a");
    
    if (rank == 0)
    {
        for (i = 0; i < nb[rank]; i++)
        {
            for (j = 0; j < JSIZE; j++)
            {
                fprintf(ff, "%f ", a[i + rank * nb1][j]);
            }
            fprintf(ff, "\n");
        }
        MPI_Send(&t_end, 1, MPI_DOUBLE, rank + 1, size + 1, MPI_COMM_WORLD);
    }
    else
    {
        printf("%d\n", rank);
        if (rank == size - 1) //чтобы последний поток ничего не принимал
        {
            MPI_Recv(&t_end, 1, MPI_DOUBLE, rank - 1, size + 1, MPI_COMM_WORLD, &status);
            for (i = 0; i < nb[rank]; i++)
            {
                for (j = 0; j < JSIZE; j++)
                {
                    fprintf(ff, "%f ", a[i + rank * nb1][j]);
                }
                fprintf(ff, "\n");
            }
        }
        else
        {
            MPI_Recv(&t_end, 1, MPI_DOUBLE, rank - 1, size + 1, MPI_COMM_WORLD, &status);
            for (i = 0; i < nb[rank]; i++)
            {
                for (j = 0; j < JSIZE; j++)
                {
                    fprintf(ff, "%f ", a[i + rank * nb1][j]);
                }
                fprintf(ff, "\n");
            }
            MPI_Send(&t_end, 1, MPI_DOUBLE, rank + 1, size + 1, MPI_COMM_WORLD);
        }
    }
    fclose(ff);
    MPI_Finalize();
    return 0;
}
