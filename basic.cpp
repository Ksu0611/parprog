 #include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define ISIZE 5000
#define JSIZE 5000

int main(int argc, char **argv)
{

    MPI_Init(&argc, &argv);
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
    // начало измерения времени
    double t_start = MPI_Wtime();
    for (i = 0; i < ISIZE; i++)
    {
        for (j = 0; j < JSIZE; j++)
        {
            a[i][j] = sin(2 * a[i][j]);
        }
    }
    double t_end = MPI_Wtime();
    double working_time = t_end - t_start;
    printf("%f\n", working_time);
    // окончание измерения времени
    ff = fopen("basic.txt", "w");
    for (i = 0; i < ISIZE; i++)
    {
        for (j = 0; j < JSIZE; j++)
        {
            fprintf(ff, "%f ", a[i][j]);
        }
        fprintf(ff, "\n");
    }
    fclose(ff);

    MPI_Finalize();
}
