
#include <stdio.h>
#include <omp.h>

int main() {
    
    int var = 0;
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        #pragma omp flush(var)
        while (id != var);
        #pragma omp critical
        {
            var += 1;
            printf("id %d accessed var, value: %d\n", id, var);
        }
    }

    #pragma omp master
    printf("Var: %d\n", var);

    return 0;
}
