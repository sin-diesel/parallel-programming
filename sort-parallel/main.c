
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <assert.h>

enum {
    MAX_SIZE = 1000,
};

struct test_t {

    int array[MAX_SIZE];
    int size;

};

void quick_sort(int* array, int low, int high);

void dump_array(int* array, int size);

void test_init(struct test_t* test, int* arr, int arr_size) {

    test->size = arr_size;
    for (int idx = 0; idx < arr_size; ++idx) {
        test->array[idx] = arr[idx];
    }

}

void test_run(struct test_t* tests, int num_of_tests) {

    for (int idx = 0; idx < num_of_tests; ++idx) {

        printf("Before sort:\n");
        dump_array(tests[idx].array, tests[idx].size);

        quick_sort(tests[idx].array, 0, tests[idx].size);

        printf("After sort:\n");
        dump_array(tests[idx].array, tests[idx].size);

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



int main() {

    int num_of_tests = 2;

    struct test_t* tests = (struct test_t*) calloc(num_of_tests, sizeof(struct test_t));
    assert(tests != NULL);

    // worst case
    int test_0[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

    // average case
    int test_1[] = {1, 4, 2, 9, 5, 7, 3, 4, 10, 2, 3};

    test_init(&tests[0], test_0, sizeof(test_0) / sizeof(int));
    test_init(&tests[1], test_1, sizeof(test_1) / sizeof(int));

    test_run(tests, num_of_tests);  


    return 0;
}
