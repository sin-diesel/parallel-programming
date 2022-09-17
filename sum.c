
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("ERROR: missing N parameter\n");
        exit(1);
    }

    int N = atoi(argv[1]);
    printf("Number elements in sum: %d\n", N);
    
    return 0;
}
