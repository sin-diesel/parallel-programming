
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define PARALLEL

#ifdef NPARALLEL
    #undef PARALLEL
#endif

//#define DBG

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("ERROR: missing N parameter\n");
        exit(1);
    }

    int N = atoi(argv[1]);
    #ifdef DBG
    printf("Number elements in sum: %d\n", N);
    #endif
    long double sum = 0.0;
    
    // parallel version
    #ifdef PARALLEL
    #pragma omp parallel
    {
        // count start point using thread number
        int num_of_threads = omp_get_num_threads();
        int thread_num = omp_get_thread_num();
        int count = N / num_of_threads;
        int start = thread_num * count;
        int end = start + count;

        if (thread_num == num_of_threads - 1) {
            end = N;
            count = end - start;
        }
        
        #ifdef DBG
        printf("Running with %d threads\n", num_of_threads);
        printf("Thread: %d, starting point: %d, end point: %d, elements: %d\n", thread_num, start, end, count);
        #endif

        long double partial_sum = 0;
        for (int num = start + 1; num < end + 1; ++num) {
            partial_sum += 1.0 / num;
        }
        
        #pragma omp critical
        sum += partial_sum;
    }
    #endif


    #ifndef PARALLEL
    for (int num = 1; num < N + 1; ++num) {
        sum += 1.0 / num;
    }
    #endif

    #pragma omp barrier
    printf("sum = %Lg\n", sum);

    return 0;
}
