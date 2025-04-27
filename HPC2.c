#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

// Sequential Bubble Sort
void bubbleSortSequential(int arr[], int n) {
    int temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Optimized Parallel Bubble Sort
void bubbleSortParallel(int arr[], int n) {
    if (n < 2000) {  // Increased threshold to avoid instability
        bubbleSortSequential(arr, n);
        return;
    }

    int temp;
    for (int i = 0; i < n; i++) {
        #pragma omp parallel for private(temp)
        for (int j = (i % 2 == 0) ? 0 : 1; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}


// Function to generate random numbers
void generateRandomArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;
    }
}

int main() {
    srand(time(0));
    omp_set_num_threads(4); // Set OpenMP to use 4 threads

    int n;
    printf("Enter the size of the array: ");
    scanf("%d", &n);

    int *arr1 = (int *)malloc(n * sizeof(int));
    int *arr2 = (int *)malloc(n * sizeof(int));
    int *arr3 = (int *)malloc(n * sizeof(int));

    generateRandomArray(arr1, n);
    for (int i = 0; i < n; i++) {
        arr2[i] = arr1[i];
        arr3[i] = arr1[i];
    }


    printf("\n+----------------------+------------------------+------------------------+------------------+------------------+------------------+\n");
    printf("| Iteration | Bubble Sort Seq Time | Bubble Sort Par Time | Merge Sort Seq Time | Merge Sort Par Time | Speedup | Efficiency |\n");
    printf("+----------------------+------------------------+------------------------+------------------+------------------+------------------+\n");

    for (int i = 0; i < 5; i++) {
        double start_time, end_time;

        // Sequential Bubble Sort
        start_time = omp_get_wtime();
        bubbleSortSequential(arr1, n);
        end_time = omp_get_wtime();
        double bubbleSortSeqTime = end_time - start_time;

        // Parallel Bubble Sort
        start_time = omp_get_wtime();
        bubbleSortParallel(arr1, n);
        end_time = omp_get_wtime();
        double bubbleSortParTime = end_time - start_time;


        // Calculate Speedup and Efficiency
        double bubbleSortSpeedup = bubbleSortSeqTime / bubbleSortParTime;
        double efficiency = (bubbleSortSpeedup + mergeSortSpeedup) / 6;

        // Print results
        printf("| %9d | %22f | %22f | %16f | %16f | %8f | %10f |\n", 
               i + 1, bubbleSortSeqTime, bubbleSortParTime, mergeSortSeqTime, mergeSortParTime, 
               bubbleSortSpeedup + mergeSortSpeedup, efficiency);
    }

    printf("+----------------------+------------------------+------------------------+------------------+------------------+------------------+\n");

    free(arr1);
    free(arr2);
    free(arr3);

    return 0;
}
