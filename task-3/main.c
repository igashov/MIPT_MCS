#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char** argv) 
{
    for (int n_threads = 1; n_threads < 33; ++n_threads)
    {
    printf("THREADS: %d\n", n_threads);
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
    double* matrix1 = (double*)malloc(rows1 * cols1 * sizeof(double));
    double* matrix2 = (double*)malloc(rows2 * cols2 * sizeof(double));
    double* matrix2_T = (double*)malloc(rows2 * cols2 * sizeof(double));
    for (int i = 0; i < rows1 * cols1; ++i) 
    {
        fread(&matrix1[i], sizeof(double), 1, file1);
    }
    for (int i = 0; i < rows2 * cols2; ++i)
    {
        fread(&matrix2[i], sizeof(double), 1, file2);
    }
    int k = 0;
    for (int i = 0; i < cols2; ++i) 
    {
        for (int j = 0; j < rows2; ++j) 
        {
            matrix2_T[k++] = matrix2[j * cols2 + i];
        }
    }
    
    /* Allocate memory for result matrix */
    double* matrix = (double*)malloc(rows1 * cols2 * sizeof(double));
    
    /* Parallel section */
    struct timespec start;
    struct timespec finish;
    clock_gettime(CLOCK_MONOTONIC, &start);
    int total_N = rows1 * cols2;
    #pragma omp parallel num_threads(n_threads)
    {
        #pragma omp for
        for (int num = 0; num < total_N; ++num)
        {
           int row = num / cols2;
           int col = num % cols2;
           matrix[row * cols2 + col] = 0;
           for (int i = 0; i < cols1; ++i) 
           {
               matrix[row * cols2 + col] += matrix1[row * cols1 + i] * matrix2_T[col * cols2 + i];
	   } 
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &finish);
    double time = (finish.tv_sec - start.tv_sec) * 1.0 + (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("time: %f\n", time);
    
    /*
    printf("Result: %d x %d\n", rows1, cols2);
    for (int i = 0; i < rows1; ++i) 
    {
        for (int j = 0; j < cols2; ++j) 
	{
	    printf("%f ", matrix[i * cols2 + j]);
	}
	printf("\n");
    }
    */

    printf("=====================================\n");

    /* Free all acquired resources */
    free(matrix1);
    free(matrix2);
    free(matrix2_T);
    free(matrix);
    fclose(file1);
    fclose(file2);
    }
    return 0;
}
