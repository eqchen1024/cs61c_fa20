#include "ex2.h"

double dotp_naive(double* x, double* y, int arr_size) {
    double global_sum = 0.0;
    for (int i = 0; i < arr_size; i++)
        global_sum += x[i] * y[i];
    return global_sum;
}

// Critical Keyword
double dotp_critical(double* x, double* y, int arr_size) {
    double global_sum = 0.0;
    // Use the critical keyword here!
    #pragma omp parallel for
    for (int i = 0; i < arr_size; i++) {
        #pragma omp critical
        global_sum += x[i] * y[i];
    }
    return global_sum;
}

// Reduction Keyword
double dotp_reduction(double* x, double* y, int arr_size) {
    double global_sum = 0.0;
    // Use the reduction keyword here!
    #pragma omp parallel for reduction(+:global_sum)
    for (int i = 0; i < arr_size; i++) {
        global_sum += x[i] * y[i];
    }

    return global_sum;
}

// Manual Reduction
double dotp_manual_reduction(double* x, double* y, int arr_size) {
    double global_sum = 0.0;
    // Do NOT use the `reduction` directive here!
    #pragma omp parallel
    {
        int thread_ID = omp_get_thread_num();
        int thread_num = omp_get_num_threads();
        double tmp_sum = 0;
        for (int i=thread_ID; i<ARRAY_SIZE; i+=thread_num){
            tmp_sum += x[i] * y[i];
        }
        #pragma omp critical
        global_sum += tmp_sum;
    }
    return global_sum;
}
