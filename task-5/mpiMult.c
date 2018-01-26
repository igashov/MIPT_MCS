#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "mpi.h"

MPI_Status status;

int main(int argc, char **argv)
{  
    int n_threads = 5;
 
    /* Open files and read pre-information */
    char* filename1 = argv[1];
    char* filename2 = argv[2];
    FILE* file1 = fopen(filename1, "r");
    FILE* file2 = fopen(filename2, "r");
    char message[6];
    int rows1, rows2, cols1, cols2;
    fread(message, sizeof(char), 6, file1);
    fread(&rows1, sizeof(int), 1, file1);
    fread(&cols1, sizeof(int), 1, file1);
    fread(message, sizeof(char), 6, file2);
    fread(&rows2, sizeof(int), 1, file2);
    fread(&cols2, sizeof(int), 1, file2);
    
    /* Check if multiplication is possible */
    if (cols1 != rows2)
    {
        printf("Error. Incorrect shapes\n");
        return 0;
    }
    
    /* Read matrices from files */
    double* a = (double*)malloc(rows1 * cols1 * sizeof(double));
    double* b = (double*)malloc(rows2 * cols2 * sizeof(double));
    double* b_T = (double*)malloc(rows2 * cols2 * sizeof(double));
    for (int i = 0; i < rows1 * cols1; ++i)
    {
        fread(&a[i], sizeof(double), 1, file1);
    }
    for (int i = 0; i < rows2 * cols2; ++i)
    {
        fread(&b[i], sizeof(double), 1, file2);
    }
    int k = 0;
    for (int i = 0; i < cols2; ++i)
    {
        for (int j = 0; j < rows2; ++j)
        {
            b_T[k++] = b[j * cols2 + i];
        }
    }
    
    /* Allocate memory for result matrix */
    double* c = (double*)malloc(rows1 * cols2 * sizeof(double));
    
    int numtasks, taskid, numworkers, offset, rows, source, dest;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    
    numworkers = numtasks - 1;

    /*---------------------------- master ----------------------------*/
    if (taskid == 0)
    { 
        struct timespec start;
        struct timespec finish;
        clock_gettime(CLOCK_MONOTONIC, &start);

        /* Send matrix data to the worker tasks */
        rows = rows1 / numworkers;
        offset = 0;

        for (dest = 1; dest <= numworkers; ++dest)
        {
            MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            offset += rows;
        }
                
        /* Wait for results from all worker tasks */
        for (source = 1; source <= numworkers; ++source)
        {
            MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(c + offset * cols2, rows * cols2, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &status);
        }

	clock_gettime(CLOCK_MONOTONIC, &finish);
        double time = (finish.tv_sec - start.tv_sec) * 1.0 + (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
        printf("time: %f\n", time);
       
 	/*
	printf("Result\n");
	for (int i = 0; i < rows1 * cols2; ++i) 
	{
	    printf("%f ", c[i]);
	    if ((i + 1) % cols2 == 0) 
	    {
		printf("\n");
	    }
	}
	*/
    }

    /*---------------------------- worker----------------------------*/
    if (taskid > 0) {
        source = 0;
        MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);       
	if (rows1 - (offset + rows) < rows) 
	{
	    rows += rows1 - (offset + rows);
	}	

	/* Matrix multiplication */
        int total_N = rows * cols2;
        #pragma omp parallel num_threads(n_threads)
        {
            #pragma omp for
            for (int num = offset * cols2; num < offset * cols2 + total_N; ++num)
            {
                int row = num / cols2;
                int col = num % cols2;
                c[row * cols2 + col] = 0;
                for (int i = 0; i < cols1; ++i)
                {
                    c[row * cols2 + col] += a[row * cols1 + i] * b_T[col * cols2 + i];
                }
            }
        }

        MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(c + offset * cols2, rows * cols2, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    free(a);
    free(b);
    free(b_T);
    free(c);
    fclose(file1);
    fclose(file2);
    return 0;
}
