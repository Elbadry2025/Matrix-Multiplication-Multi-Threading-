#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#define MAX_LENGTH 20

double arrA[MAX_LENGTH][MAX_LENGTH];
double arrB[MAX_LENGTH][MAX_LENGTH];
double arrC[MAX_LENGTH][MAX_LENGTH];
double arrC2[MAX_LENGTH][MAX_LENGTH];
double arrC3[MAX_LENGTH][MAX_LENGTH];

int rowsA;
int columnsA;
int rowsB;
int columnsB;

char *z;

struct timeval stop1, start1;
struct timeval stop2, start2;
struct timeval stop3, start3;

struct
{
    int rowIndex;
    int colIndex;

} typedef matrix_minimized;

void Take_Input(char *ar[])
{
    // Declare pointers to char for file names
    char *file_a;
    char *file_b;

    // Check if command line arguments are provided
    if (ar[1] != NULL && ar[2] != NULL)
    {
        // Assign command line arguments to file names
        file_a = ar[1];
        file_b = ar[2];
        z = ar[3];
    }
    else
    {
        // Assign default file names if command line arguments are not provided
        file_a = "a";
        file_b = "b";
        z = "c";
    }

    // Declare file pointer
    FILE *fp;

    // Open file_a in read mode
    fp = fopen(file_a, "r");

    // Check if file is opened successfully
    if (fp == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    // Read the dimensions of the matrix from the first line of file_a
    fscanf(fp, "row=%d col=%d", &rowsA, &columnsA);

    // Read the elements of the matrix from the file
    for (int i = 0; i < rowsA; i++)
    {
        for (int j = 0; j < columnsA; j++)
        {
            fscanf(fp, "%lf", &arrA[i][j]);
        }
    }

    // Close file_a
    fclose(fp);

    // Open file_b in read mode
    fp = fopen(file_b, "r");

    // Check if file is opened successfully
    if (fp == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    // Read the dimensions of the matrix from the first line of file_b
    fscanf(fp, "row=%d col=%d", &rowsB, &columnsB);

    // Read the elements of the matrix from the file
    for (int i = 0; i < rowsB; i++)
    {
        for (int j = 0; j < columnsB; j++)
        {
            fscanf(fp, "%lf", &arrB[i][j]);
        }
    }

    // Close file_b
    fclose(fp);
}

void *CaseA()
{
    // Calculate the elements of arrC as a matrix product of arrA and arrB
    for (int i = 0; i < rowsA; i++)
    {
        for (int j = 0; j < columnsB; j++)
        {
            arrC[i][j] = 0;
            for (int k = 0; k < rowsB; k++)
            {
                arrC[i][j] += arrA[i][k] * arrB[k][j];
            }
        }
    }

    // Exit the thread
    pthread_exit(NULL);
}

void *CaseB(void *arg)
{
    // Cast the argument to a pointer to the matrix_minimized struct
    matrix_minimized *matrix = (matrix_minimized *)arg;

    // Initialize the elements of arrC2 for this thread's row index to 0
    for (int k = 0; k < columnsA; k++)
    {
        arrC2[matrix->rowIndex][k] = 0;
    }

    // Calculate the product of arrA and arrB for this thread's row index
    for (int i = 0; i < columnsB; i++)
    {
        for (int j = 0; j < rowsB; j++)
        {
            arrC2[matrix->rowIndex][i] += arrA[matrix->rowIndex][j] * arrB[j][i];
        }
    }
    // Exit the thread
    pthread_exit(NULL);
}

void *CaseC(void *arg)
{
    // Cast the argument to a pointer to the matrix_minimized struct
    matrix_minimized *matrix3 = (matrix_minimized *)arg;

    // Initialize the element of arrC3 for this thread's row and column index to 0
    arrC3[matrix3->rowIndex][matrix3->colIndex] = 0;

    // Calculate the product of arrA and arrB for the corresponding element of arrC3
    for (int i = 0; i < columnsA; i++)
    {
        arrC3[matrix3->rowIndex][matrix3->colIndex] += arrA[matrix3->rowIndex][i] * arrB[i][matrix3->colIndex];
    }
    // Exit the thread
    pthread_exit(NULL);
}

void print_Output(double mat[][MAX_LENGTH], char methodName[], __u_long t, __u_long ts)
{
    char *fileName;
    strcpy(fileName, z); // assign filename to z which was set in Take_Input()

    int threadsNumber = 0;
    char *cat;
    if (strcmp(methodName, "Method: Case A (per matrix)") == 0)
    {
        cat = "_per_matrix";
        strcat(fileName, cat); // append _per_matrix to filename
        threadsNumber = 1;
    }
    else if (strcmp(methodName, "Method: Case B (per row)") == 0)
    {
        cat = "_per_row";
        strcat(fileName, cat); // append _per_row to filename
        threadsNumber = rowsA;
    }
    else if (strcmp(methodName, "Method: Case C (per element)") == 0)
    {
        cat = "_per_element";
        strcat(fileName, cat); // append _per_element to filename
        threadsNumber = rowsA * columnsB;
    }

    FILE *f = fopen(fileName, "w"); // open the file with the filename created above in write mode

    // print method name, matrix dimensions, threads number, and time taken to console and file
    fprintf(f, "%s\n", methodName);
    printf("%s\n", methodName);
    fprintf(f, "row = %d col = %d\nthreads = %d time in microseconds= %luus, time in seconds = %lus\n", rowsA, columnsB, threadsNumber, t, ts);
    printf("row = %d col = %d\nthreads = %d time in microseconds= %luus, time in seconds = %lus\n", rowsA, columnsB, threadsNumber, t, ts);

    // print the matrix elements to console and file
    for (int i = 0; i < rowsA; i++)
    {
        for (int j = 0; j < columnsB; j++)
        {
            fprintf(f, "%.3lf   ", mat[i][j]);
            printf("%.3lf   ", mat[i][j]);
        }
        fprintf(f, "\n");
        printf("\n");
    }

    fclose(f); // close the file
}

void free_Resoruces(matrix_minimized *m, matrix_minimized **m3)
{
    free(m); // Free memory for the matrix_minimized array
    for (int i = 0; i < rowsA; i++)
    {
        free(m3[i]); // Free memory for each matrix_minimized struct in the array
    }
    free(m3); // Free memory for the matrix_minimized array
}

int main(int argc, char *argv[])
{

    // Parse command line arguments to obtain input matrices A and B
    Take_Input(argv);

    // Start timer for Case A
    gettimeofday(&start1, NULL); // start checking time

    // Create thread for Case A
    pthread_t thread1;
    pthread_create(&thread1, NULL, CaseA, NULL);
    pthread_join(thread1, NULL);
    // Stop the timer to record the execution time
    gettimeofday(&stop1, NULL);

    // Start timer for Case B
    gettimeofday(&start2, NULL); // start checking time

    // Create threads for Case B
    matrix_minimized *m = (matrix_minimized *)malloc(rowsA * sizeof(matrix_minimized));
    pthread_t threads2[rowsA];
    for (int i = 0; i < rowsA; i++)
    {
        m[i].rowIndex = i;
        pthread_create(&threads2[i], NULL, CaseB, (void *)&m[i]);
    }
    for (int i = 0; i < rowsA; i++)
    {
        pthread_join(threads2[i], NULL);
    }
    // Stop the timer to record the execution time
    gettimeofday(&stop2, NULL);

    // Start timer for Case C
    gettimeofday(&start3, NULL); // start checking time

    // Create threads for Case C
    matrix_minimized **m3 = (matrix_minimized **)malloc(rowsA * sizeof(matrix_minimized *));
    for (int i = 0; i < rowsA; i++)
    {
        m3[i] = (matrix_minimized *)malloc(columnsB * sizeof(matrix_minimized));
    }
    pthread_t threads3[rowsA][columnsB];
    for (int i = 0; i < rowsA; i++)
    {
        for (int j = 0; j < columnsB; j++)
        {
            m3[i][j].rowIndex = i;
            m3[i][j].colIndex = j;
            pthread_create(&threads3[i][j], NULL, CaseC, (void *)&m3[i][j]);
        }
    }

    // Wait for threads to finish before proceeding
    for (int i = 0; i < rowsA; i++)
    {
        for (int j = 0; j < columnsB; j++)
        {
            pthread_join(threads3[i][j], NULL);
        }
    }
    // Stop the timer to record the execution time
    gettimeofday(&stop3, NULL);
    // Free allocated memory
    free_Resoruces(m, m3);

    // Print output matrices and execution times for each case
    print_Output(arrC, "Method: Case A (per matrix)", stop1.tv_usec - start1.tv_usec, stop1.tv_sec - start1.tv_sec);
    print_Output(arrC2, "Method: Case B (per row)", stop2.tv_usec - start2.tv_usec, stop2.tv_sec - start2.tv_sec);
    print_Output(arrC3, "Method: Case C (per element)", stop3.tv_usec - start3.tv_usec, stop3.tv_sec - start3.tv_sec);

    return 0;
}
