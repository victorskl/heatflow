/*
Compile with
    gcc heatflow_gauss_sequential.cpp -o heatflow_gauss_sequential.exe

Run with
    heatflow_gauss_sequential.exe 10 10 4 4

Whereas
    first argument is number of points for square region
    second argument is number of iteration
    third and fourth arguments form a heat point
*/
#include <cstdio>
#include <cstdlib>
//#include <omp.h>
#define LOADCONS 2
#define SIZE 10000
#define MAX 100
//#define HEATX 4
//#define HEATY 4
int HEATX, HEATY;
double matrix[SIZE][SIZE];

void initialize(int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (i == HEATX&&j == HEATY)

            {
                matrix[i][j] = 1000;
            }
            else
            {
                matrix[i][j] = 0;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    int max;
    int size;
    sscanf(argv[1], "%d", &size);
    sscanf(argv[2], "%d", &max);
    sscanf(argv[3], "%d", &HEATX);
    sscanf(argv[4], "%d", &HEATY);
    printf("Number of Points     : %d\n", size);
    printf("Number of Iterations : %d\n", max);
    printf("Heat Point is at     : (%d, %d)\n", HEATX, HEATY);
    //double start_time;
    //double end_time;
    initialize(size);
    //start_time = omp_get_wtime();
    for (int n = 0; n < max; n++)
    {
        for (int i = 1; i < size - 1; i++)
        {
            for (int j = 1; j < size - 1; j++)
            {
                if (i != HEATX || j != HEATY)
                {
                    matrix[i][j] = (matrix[i - 1][j] + matrix[i + 1][j] + matrix[i][j - 1] + matrix[i][j + 1]) / 4;
                }
            }
        }
    }
    //end_time = omp_get_wtime();
    //printf("Gauss sequential takes %f times", end_time - start_time);

    FILE *f = fopen("heatflow_gauss_sequential.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            fprintf(f, "%.2f,", matrix[i][j]);
            //printf("%.2f,",matrix[i][j]);
        }
        fprintf(f, "\n ");
    }

    printf("Output save to       : heatflow_gauss_sequential.txt\n");

    return 0;
}