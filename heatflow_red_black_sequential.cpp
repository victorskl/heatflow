/*
Compile with
    gcc heatflow_red_black_sequential.cpp -o heatflow_red_black_sequential.exe

Run with
    heatflow_red_black_sequential.exe 10 10 4 4

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
double previous[SIZE][SIZE];

void initialize(int size)
{
    for (int i = 0; i <= size; i++)
    {
        for (int j = 0; j <= size; j++)
        {
            if (i == HEATX&&j == HEATY)

            {
                matrix[i][j] = 1000;
                previous[i][j] = 1000;
            }
            else
            {
                matrix[i][j] = 0;
                previous[i][j] = 0;
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
    //double start_time,end_time;
    initialize(size);
    //start_time=omp_get_wtime();
    for (int n = 0; n < max; n++)
    {
        for (int i = 1; i < size - 1; i++)
        {
            for (int j = i % 2 + 1; j < size - 1; j = j + 2)
            {
                if (i != HEATX || j != HEATY)
                {
                    matrix[i][j] = (matrix[i - 1][j] + matrix[i + 1][j] + matrix[i][j - 1] + matrix[i][j + 1]) / 4;
                }
            }
        }
        // do for 0,1 cell
        for (int i = 1; i < size - 1; i++)
        {
            for (int j = 2 - i % 2; j < size - 1; j = j + 2)
            {
                if (i != HEATX || j != HEATY)
                {
                    matrix[i][j] = (matrix[i - 1][j] + matrix[i + 1][j] + matrix[i][j - 1] + matrix[i][j + 1]) / 4;
                }
            }
        }

    }
    //end_time=omp_get_wtime();
    //printf("Black and white sequential takes %f times",end_time-start_time);

    FILE *f = fopen("heatflow_red_black_sequential.txt", "w");
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

    printf("Output save to       : heatflow_red_black_sequential.txt\n");

    return 0;
}