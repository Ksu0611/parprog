 #include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <omp.h>

const int ISIZE = 5000;
const int JSIZE = 5000;


int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("example: ./basic_omp [Nthreads]\n");
        return 1;
    }
    const int NTHREADS = atoi(argv[1]);
    double** a = (double **)calloc(ISIZE, sizeof(double**));
    if (!a)
    {
        printf("Not enough memory");
        return 1;
    }
    for(int i = 0; i < ISIZE; ++i)
    {
        a[i] = (double*)calloc(JSIZE, sizeof(double*));
        if (!a[i])
        {
            printf("Not enough memory");
            return 1;
        }
    }

    FILE *ff;

    //подготовительная часть – заполнение некими данными
    for (int i = 0; i < ISIZE; i++)
    {
        for (int j=0; j < JSIZE; j++)
        {
            a[i][j] = 10*i +j;
        }
    }

    // требуется обеспечить измерение времени работы данного цикла
    double start = omp_get_wtime();

    #pragma omp parallel for num_threads(NTHREADS)
    for (int i = 0; i < ISIZE; i++)
    {
        for (int j = 0; j < JSIZE; j++)
        {
            a[i][j] = sin(2*a[i][j]);
        }
    }

    double stop = omp_get_wtime();

#ifdef TEST

    ff = fopen("result_openmp.txt","w");
    for(int i=0; i < ISIZE; i++)
    {
        for (int j=0; j < JSIZE; j++)
        {
            fprintf(ff,"%f ",a[i][j]);
        }
        fprintf(ff,"\n");
    }
    fclose(ff);

#endif

    printf("Time spent: %f sec\n", (stop - start));

    for(int i = 0; i < ISIZE; i++)
    {
        free(a[i]);
    }
    free(a);
}
