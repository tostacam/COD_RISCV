#include <stdio.h>
#include <stdlib.h>
#include "../utilities/matrix_utils.h"
#include "../utilities/benchmark.h"

typedef struct {
  matrix A;
  matrix BT;
  matrix C;
} dgemm_args;

#include <arm_neon.h> /* for MacOS M3 ARM */

void dgemm(void* args_ptr){
  dgemm_args* args = (dgemm_args*) args_ptr;
  matrix A = args->A;
  matrix BT = args->BT;
  matrix C = args->C;

  size_t n = A.rows;
  for(size_t i = 0; i < n; ++i){
    for(size_t j = 0; j < n; ++j){

      float64x2_t sum_vec = vdupq_n_f64(0.0);

      for(size_t k = 0; k < n; k += 2){
        // loading 2 doubles (16 bytes) from Matrix A
        float64x2_t a = vld1q_f64(&A.data[i*n + k]);

        // loading 2 doubles (16 bytes) from Matrix B
        float64x2_t b = vld1q_f64(&BT.data[j*n + k]);

        // sum += a * b
        sum_vec = vfmaq_f64(sum_vec, a, b);
      }

      // reduction
      double sum = vgetq_lane_f64(sum_vec, 0) + vgetq_lane_f64(sum_vec, 1);

      C.data[i*n+j] = sum;
    }
  }
}

int main(int argc, char** argv){
  int size = atoi(argv[1]);

  char filenameA[256], filenameB[256], filenameC[256];
  snprintf(filenameA, sizeof(filenameA), "./datasets/matrix_%dx%d_A.bin", size, size);
  snprintf(filenameB, sizeof(filenameB), "./datasets/matrix_%dx%d_B.bin", size, size);
  snprintf(filenameC, sizeof(filenameC), "./datasets/matrix_%dx%d_C_output.bin", size, size);
  
  FILE* fileA = fopen(filenameA, "rb");
  FILE* fileB = fopen(filenameB, "rb");

  matrix A = read_matrix(size, fileA);
  matrix B = read_matrix(size, fileB);
  matrix BT = transpose_matrix(B);
 
  matrix C;
  C.rows = A.rows;
  C.cols = A.cols;
  C.data = calloc(C.rows * C.cols, sizeof(double));

  dgemm_args args = { .A = A, .BT = BT, .C = C };

  double elapsed = benchmark(dgemm, &args);

  //print_matrix(A);
  //print_matrix(B);
  //print_matrix(C);

  FILE* fileC = fopen(filenameC, "wb");
  fwrite(C.data, sizeof(double), C.rows * C.cols, fileC);
  fclose(fileC);

  // printing raw timing to read from python
  printf("%f\n", elapsed);

  release_matrix(A);
  release_matrix(B);
  release_matrix(C);

  return 0;  
}

/* 
Book example with AVX

#include <x86intrin.h>
void dgemm(size_t n, double* A, double* B, double* C){
  for(size_t i = 0; i < n; i+=4){
    for(size_t j = 0; j < n; ++j){
      __m256d c0 = _mm256_load_pd(C+i+j*n); // c0 = C[i][j] 
      for(size_t k = 0; k < n; ++k){
        c0 = _mm256_add_pd(c0,  // c0 += A[i][k]*B[k][j]
              _mm256_mul_pd(_mm256_load_pd(A+i+k*n),
              _mm256_broadcast_sd(B+k+j*n)));
        _mm256_store_pd(C+i+j*n, c0); // C[i][j] = c0
      }
    }
  }
}
*/


