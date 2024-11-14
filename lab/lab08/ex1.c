#include "ex1.h"

void v_add_naive(double* x, double* y, double* z) {
    #pragma omp parallel
    {
        for(int i=0; i<ARRAY_SIZE; i++)
            z[i] = x[i] + y[i];
    }
}

// Adjacent Method
void v_add_optimized_adjacent(double* x, double* y, double* z) {
    // Do NOT use the `for` directive here!
    #pragma omp parallel
    {
        int thread_ID = omp_get_thread_num();
        int thread_num = omp_get_num_threads();
        for (int i=thread_ID; i<ARRAY_SIZE; i+=thread_num){
            z[i] = x[i] + y[i];
        }
    }

}

// Chunks Method
void v_add_optimized_chunks(double* x, double* y, double* z) {
    // Do NOT use the `for` directive here!
    #pragma omp parallel
    {
        int thread_ID = omp_get_thread_num();
        int thread_num = omp_get_num_threads();
        int chunk_size = ARRAY_SIZE/thread_num;
        for (int i=thread_ID*chunk_size; i<thread_ID*chunk_size + chunk_size; i++){
            z[i] = x[i] + y[i];
        }
        if (thread_ID == (thread_num - 1)) {
            for (int i=thread_num*chunk_size; i<ARRAY_SIZE; i++){
                z[i] = x[i] + y[i];
            }
        }
    }
}
