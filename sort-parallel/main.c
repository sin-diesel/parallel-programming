
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <assert.h>
#include <time.h>

enum {
    MAX_SIZE = 1000,
};

struct test_t {

    int array[MAX_SIZE];
    int size;

};

typedef void (*init_case)(int*, int);

void quick_sort(int* array, int low, int high);

void dump_array(int* array, int size);

void test_init(struct test_t* test, int* arr, int arr_size) {

    test->size = arr_size;
    for (int idx = 0; idx < arr_size; ++idx) {
        test->array[idx] = arr[idx];
    }

}


void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int partition(int* array, int low, int high) {
  
    int pivot = array[high];
    int i = (low - 1);

    for (int j = low; j < high; j++) {

        if (array[j] <= pivot) {
            i++;
            swap(&array[i], &array[j]);
        }

    }

    swap(&array[i + 1], &array[high]);
  
    return (i + 1);
}

void quick_sort(int* array, int low, int high) {

    if (low < high) {
    
        int pi = partition(array, low, high);
    
        quick_sort(array, low, pi - 1);
        quick_sort(array, pi + 1, high);

    }

}

void dump_array(int* array, int size) {

    for (int i = 0; i < size; ++i) {
        printf("%d  ", array[i]);
    }

    printf("\n");

}

void init_worst_case(int* test_arr, int size) {

    for (int i = 0; i < size; ++i) {
        test_arr[i] = size - i;
    }

}

void init_average_case(int* test_arr, int size) {

    for (int i = 0; i < size; ++i) {
        test_arr[i] = rand() % size;
    }

}


void test_run(int start_size, int end_size, init_case test_case) {

    int start_time = 0;
    int end_time = 0;
    double test_time = 0;

    FILE* output = fopen("main.dat", "a+");
    assert(output != NULL);

    fprintf(output, "array_size    time(s) (%s)\n", __func__);

    for (int i = start_size; i < end_size; ++i) {

        int* test_arr = (int*) calloc(i, sizeof(int));
        assert(test_arr != NULL);
        test_case(test_arr, i);

        start_time = clock();
        quick_sort(test_arr, 0, i);
        end_time = clock();

        test_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
        printf("test_time for test %d is %5lg\n", i, test_time);
        fprintf(output, "%4d          %4lg\n", i, test_time);


    }

    fprintf(output, "\n\n\n");
    fclose(output);

}




int main() {  

    srand(time(NULL));

    printf("WORST CASE TESTING\n");
    test_run(0, 1000, &init_worst_case);
    printf("BEST CASE TESTING\n");
    test_run(0, 1000, &init_average_case);

    return 0;
}
