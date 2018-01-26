#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) 
{
    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);
    char* file_name = argv[3];
    FILE* f = fopen(file_name, "w");
    char message[] = "MATRIX";
    fwrite(message, sizeof(char), 6, f);
    fwrite(&rows, sizeof(int), 1, f);
    fwrite(&cols, sizeof(int), 1, f);
    double random_value;
    srand(time(NULL));
    for (int i = 0; i < cols; ++i) 
    {
        for (int j = 0; j < rows; ++j) 
        {
            random_value = (double)rand() / RAND_MAX * 2.0 - 1.0;
            fwrite(&random_value, sizeof(double), 1, f);
//            printf("%f ", random_value);
        }
    }
    fclose(f);
    return 0;
}
