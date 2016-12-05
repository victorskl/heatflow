/*
Compile with
    mpic++ heatflow_gauss_mpi.cpp -o heatflow_gauss_mpi.exe

Run with
    mpiexec -np 4 heatflow_gauss_mpi.exe 10 10 4 4

Whereas
    first argument is number of points for square region
    second argument is number of iteration
    third and fourth arguments form a heat point
*/
#include <cstdio>
#include <cstdlib>
#include <mpi.h>
#define SIZE 10000
//#define HEATX 4
//#define HEATY 4
int HEATX, HEATY;
//double matrix[SIZE*SIZE];
double* matrix;

void initialize(int size1)
{
    for (int k = 1; k <= (size1 + 1)*(size1 + 1); k++)
    {
        int i = (k - 1) / (size1 + 1);
        int j = k % (size1 + 1) - 1;
        if (j == -1) j = size1 + 1;
        if (i == HEATX&&j == HEATY)
        {
            matrix[k] = 1000;
            //matrix[k]=k;
        }
        else
        {
            matrix[k] = 0;
            //matrix[k]=k;
        }
    }
}

int main(int argc, char *argv[]) {
    int max;
    int size;
    int nprocess;
    int myid;

    double start_time;
    double end_time;


    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) {
        fprintf(stderr, "MPI initialization error\n");
        exit(EXIT_FAILURE);
    }
    MPI_Comm_size(MPI_COMM_WORLD, &nprocess);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    sscanf(argv[1], "%d", &size);
    sscanf(argv[2], "%d", &max);
    sscanf(argv[3], "%d", &HEATX);
    sscanf(argv[4], "%d", &HEATY);

    //int chunksize[1000];
    //int chunkid[1000];
    matrix = (double *)malloc(SIZE*SIZE * sizeof(double));
    int *chunksize = (int *)malloc(SIZE * sizeof(int));
    int *chunkid = (int *)malloc(SIZE * sizeof(int));

    if ((size + 1) % nprocess == 0)
    {
        chunksize[0] = (size + 1)*((size + 1) / nprocess);
        chunkid[0] = 0;
        for (int k = 1; k < nprocess; k++)
        {
            chunksize[k] = (size + 1)*((size + 1) / nprocess);
            chunkid[k] = chunkid[k - 1] + chunksize[k - 1];
        }
    }
    else
    {
        int k = 0;
        chunkid[k] = 0;
        chunksize[k] = (size + 1)*((size + 1) / nprocess);
        for (k = 1; k < nprocess - 1; k++)
        {
            chunksize[k] = (size + 1)*((size + 1) / nprocess);
            chunkid[k] = chunkid[k - 1] + chunksize[k - 1];
        }
        chunksize[k] = (size + 1)*((size + 1) - ((size + 1) / nprocess)*k);
        chunkid[k] = chunkid[k - 1] + chunksize[k - 1];
    }

    if (myid == 0)
    {
        start_time = MPI_Wtime();
        initialize(size);

        printf("Number of Points     : %d\n", size);
        printf("Number of Iterations : %d\n", max);
        printf("Heat Point is at     : (%d, %d)\n", HEATX, HEATY);
    }

    //int localmatrix[SIZE];
    double* localmatrix = (double *)malloc((size+1)*(size+1) * sizeof(double));
    if (MPI_Scatterv(matrix, chunksize, chunkid, MPI_DOUBLE, localmatrix, chunksize[myid], MPI_DOUBLE, 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
        perror("Scatter error");
        exit(1);
    }

    if (myid == 0)
    {
        double after[SIZE];
        double before[SIZE];
        for (int k = 0; k < size + 1; k++)
        {
            after[k] = 0;
            before[k] = 0;
        }
        for (int n = 0; n < max; n++)
        {
            MPI_Request request;
            MPI_Status status;
            for (int k = size + 1; k < chunksize[myid]; k++)
            {
                if (k % (size + 1) != 0 && k % (size + 1) != size)
                {
                    if (k < chunksize[myid] - size)
                    {
                        if (localmatrix[k] != 1000) localmatrix[k] = 0.25*(localmatrix[k - 1] + localmatrix[k + 1] + localmatrix[k + size + 1] + localmatrix[k - size - 1]);
                    }
                    else
                    {
                        if (localmatrix[k] != 1000) localmatrix[k] = 0.25*(localmatrix[k - 1] + localmatrix[k + 1] + after[k % (size + 1)] + localmatrix[k - size - 1]);
                    }
                }
            }
            MPI_Isend(&localmatrix[chunksize[myid] - size], size + 1, MPI_DOUBLE, myid + 1, 0, MPI_COMM_WORLD, &request);
            MPI_Irecv(after, size + 1, MPI_DOUBLE, myid + 1, 0, MPI_COMM_WORLD, &request);
            MPI_Wait(&request, &status);
        }
    }
    else if (myid == nprocess - 1)
    {
        double after[SIZE];
        double before[SIZE];
        for (int k = 0; k < size + 1; k++)
        {
            after[k] = 0;
            before[k] = 0;
        }
        for (int n = 0; n < max; n++)
        {
            MPI_Request request;
            MPI_Status status;

            for (int k = 1; k < size; k++)
            {
                if (localmatrix[k] != 1000)localmatrix[k] = 0.25*(localmatrix[k - 1] + localmatrix[k + 1] + localmatrix[k + size + 1] + before[k % (size + 1)]);
            }

            for (int k = size + 1; k < chunksize[myid] - size - 1; k++)
            {
                if (k % (size + 1) != 0 && k % (size + 1) != size)
                {
                    if (localmatrix[k] != 1000)localmatrix[k] = 0.25*(localmatrix[k - 1] + localmatrix[k + 1] + localmatrix[k + size + 1] + localmatrix[k - size - 1]);
                }
            }
            MPI_Isend(localmatrix, size + 1, MPI_DOUBLE, myid - 1, 0, MPI_COMM_WORLD, &request);
            MPI_Irecv(before, size + 1, MPI_DOUBLE, myid - 1, 0, MPI_COMM_WORLD, &request);
            MPI_Wait(&request, &status);
        }
    }
    else
    {
        double after[SIZE];
        double before[SIZE];
        for (int k = 0; k < size + 1; k++)
        {
            after[k] = 0;
            before[k] = 0;
        }
        for (int n = 0; n < max; n++)
        {
            MPI_Request beforerequest;
            MPI_Request afterrequest;
            MPI_Status afterstatus;
            MPI_Status beforestatus;
            for (int k = 1; k < size; k++)
            {
                if (localmatrix[k] != 1000)localmatrix[k] = 0.25*(localmatrix[k - 1] + localmatrix[k + 1] + localmatrix[k + size + 1] + before[k % (size + 1)]);
            }
            for (int k = size + 2; k < chunksize[myid] - size; k++)
            {
                if (k % (size + 1) != 0 && k % (size + 1) != size)
                {
                    if (localmatrix[k] != 1000)localmatrix[k] = 0.25*(localmatrix[k - 1] + localmatrix[k + 1] + localmatrix[k + size + 1] + localmatrix[k - size - 1]);
                }
            }
            for (int k = chunksize[myid] - size; k < chunksize[myid] - 1; k++)
            {
                if (localmatrix[k] != 1000)localmatrix[k] = 0.25*(localmatrix[k - 1] + localmatrix[k + 1] + after[k % (size + 1)] + localmatrix[k - size - 1]);
            }
            MPI_Isend(localmatrix, size + 1, MPI_DOUBLE, myid - 1, 0, MPI_COMM_WORLD, &beforerequest);
            MPI_Isend(&localmatrix[chunksize[myid] - size], size + 1, MPI_DOUBLE, myid + 1, 0, MPI_COMM_WORLD, &afterrequest);
            MPI_Irecv(after, size + 1, MPI_DOUBLE, myid + 1, 0, MPI_COMM_WORLD, &afterrequest);
            MPI_Irecv(before, size + 1, MPI_DOUBLE, myid - 1, 0, MPI_COMM_WORLD, &beforerequest);
            MPI_Wait(&afterrequest, &afterstatus);
            MPI_Wait(&beforerequest, &beforestatus);
        }
    }

    if (MPI_Gatherv(localmatrix, chunksize[myid], MPI_DOUBLE, matrix, chunksize, chunkid, MPI_DOUBLE, 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
        perror("Gather error");
        exit(1);
    }

    if (myid == 0)
    {
        end_time = MPI_Wtime();
        printf("Total elapse time    : %f seconds\n", end_time - start_time);

        FILE *f = fopen("heatflow_gauss_mpi.txt", "w");
        if (f == NULL)
        {
            printf("Error opening file!\n");
            exit(1);
        }
        for (int k = 0; k < (size + 1)*(size + 1); k++)
        {
            int j = k % (size + 1);
            fprintf(f, "%.2f,", matrix[k]);
            if (j == size)fprintf(f, "\n");
        }

        printf("Output save to       : heatflow_gauss_mpi.txt\n");
    }

    free(chunkid);
    free(chunksize);
    free(matrix);
    MPI_Finalize();
    return 0;
}