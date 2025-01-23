#include <omp.h>
#include <x86intrin.h>
#include <stdio.h>
#include <omp.h>
#include "compute.h"

// Computes the convolution of two matrices
int convolve(matrix_t *a_matrix, matrix_t *b_matrix, matrix_t **output_matrix) {
  // TODO: convolve matrix a and matrix b, and store the resulting matrix in
  // output_matrix
  if (a_matrix->rows < b_matrix->rows){return -1;}
  if (a_matrix->cols < b_matrix->cols){return -1;}
  *output_matrix = (matrix_t *)malloc(sizeof(matrix_t));
  int32_t *data = (int32_t *) malloc((a_matrix->rows-b_matrix->rows+1)*(a_matrix->cols-b_matrix->cols+1)*sizeof(int32_t));
  // uint32_t output_index = 0;
  #pragma omp parallel for
  for (uint32_t i=0; i<(a_matrix->rows-b_matrix->rows+1);i++){
    for (uint32_t j=0; j<(a_matrix->cols-b_matrix->cols+1);j++){
        int32_t cur_sum = 0;
	for (uint32_t k=0;k<(((b_matrix->rows)*(b_matrix->cols))/8*8);k+=8){
          // optimize
          __m256i vec_a = _mm256_set_epi32(a_matrix->data[(i*(a_matrix->cols)+j)+((k+0)/(b_matrix->cols)*(a_matrix->cols))+((k+0)%(b_matrix->cols))],
                                          a_matrix->data[(i*(a_matrix->cols)+j)+((k+1)/(b_matrix->cols)*(a_matrix->cols))+((k+1)%(b_matrix->cols))],
                                          a_matrix->data[(i*(a_matrix->cols)+j)+((k+2)/(b_matrix->cols)*(a_matrix->cols))+((k+2)%(b_matrix->cols))],
                                          a_matrix->data[(i*(a_matrix->cols)+j)+((k+3)/(b_matrix->cols)*(a_matrix->cols))+((k+3)%(b_matrix->cols))],
                                          a_matrix->data[(i*(a_matrix->cols)+j)+((k+4)/(b_matrix->cols)*(a_matrix->cols))+((k+4)%(b_matrix->cols))],
                                          a_matrix->data[(i*(a_matrix->cols)+j)+((k+5)/(b_matrix->cols)*(a_matrix->cols))+((k+5)%(b_matrix->cols))],
                                          a_matrix->data[(i*(a_matrix->cols)+j)+((k+6)/(b_matrix->cols)*(a_matrix->cols))+((k+6)%(b_matrix->cols))],
                                          a_matrix->data[(i*(a_matrix->cols)+j)+((k+7)/(b_matrix->cols)*(a_matrix->cols))+((k+7)%(b_matrix->cols))]);
	  __m256i vec_b = _mm256_loadu_si256((__m256i const *)(b_matrix->data+((b_matrix->rows)*(b_matrix->cols)-8*(k/8+1))));
	  //__m256i idx = _mm256_setr_epi32(7, 6, 5, 4, 3, 2, 1, 0);
          //__m256i reversed_vec_b = _mm256_permutevar8x32_epi32(vec_b, idx);
          __m256i mul_vec = _mm256_mullo_epi32(vec_a,vec_b);
	  int32_t tmp_arr[8];
          _mm256_storeu_si256((__m256i *) tmp_arr, mul_vec);
	  for(int h=0; h<8; h++){
            cur_sum += tmp_arr[h];
          }
	 
        }
        for (uint32_t l=((b_matrix->rows)*(b_matrix->cols))/8*8;l<(b_matrix->rows)*(b_matrix->cols);l++) {
          cur_sum += b_matrix->data[(b_matrix->rows)*(b_matrix->cols)-l-1] * a_matrix->data[(i*(a_matrix->cols)+j)+(l/(b_matrix->cols)*(a_matrix->cols))+(l%(b_matrix->cols))];
        }
	data[i*(a_matrix->cols-b_matrix->cols+1)+j] = cur_sum;
        // data[output_index] = cur_sum;
        // output_index += 1;
    }
  }
  (*output_matrix)->data=data;
  (*output_matrix)->rows=a_matrix->rows-b_matrix->rows+1;
  (*output_matrix)->cols=a_matrix->cols-b_matrix->cols+1;
  return 0;
}

// Executes a task
int execute_task(task_t *task) {
  matrix_t *a_matrix, *b_matrix, *output_matrix;

  char *a_matrix_path = get_a_matrix_path(task);
  if (read_matrix(a_matrix_path, &a_matrix)) {
    printf("Error reading matrix from %s\n", a_matrix_path);
    return -1;
  }
  free(a_matrix_path);

  char *b_matrix_path = get_b_matrix_path(task);
  if (read_matrix(b_matrix_path, &b_matrix)) {
    printf("Error reading matrix from %s\n", b_matrix_path);
    return -1;
  }
  free(b_matrix_path);

  if (convolve(a_matrix, b_matrix, &output_matrix)) {
    printf("convolve returned a non-zero integer\n");
    return -1;
  }

  char *output_matrix_path = get_output_matrix_path(task);
  if (write_matrix(output_matrix_path, output_matrix)) {
    printf("Error writing matrix to %s\n", output_matrix_path);
    return -1;
  }
  free(output_matrix_path);

  free(a_matrix->data);
  free(b_matrix->data);
  free(output_matrix->data);
  free(a_matrix);
  free(b_matrix);
  free(output_matrix);
  return 0;
}

