#include <stdio.h>
#include <stdlib.h>
#include "../utilities/matrix_utils.h"
#include "../utilities/benchmark.h"

#define MATRIX_SIZE 1024

typedef struct {
  matrix A;
  matrix BT;
  matrix C;
} dgemm_args;

#include <arm_neon.h> /* for MacOS M3 ARM */
#define UNROLL 4

void dgemm(void* args_ptr){
  dgemm_args* args = (dgemm_args*) args_ptr;
  matrix A = args->A;
  matrix BT = args->BT;
  matrix C = args->C;

  size_t n = A.rows;
  for(size_t i = 0; i < n; ++i){
    for(size_t j = 0; j < n; ++j){
      float64x2_t sum[UNROLL];
      
      for(int r = 0; r < UNROLL; ++r)
        sum[r] = vdupq_n_f64(0.0);

      float64x2_t a[UNROLL], b[UNROLL];
      for(size_t k = 0; k < n; k += 2 * UNROLL){
        
        for(int r = 0; r < UNROLL; ++r){
          a[r] = vld1q_f64(&A.data[i*n + k + r*2]);
          b[r] = vld1q_f64(&BT.data[j*n + k + r*2]);
        }

        for(int r = 0; r < UNROLL; ++r)
          sum[r] = vfmaq_f64(sum[r], a[r], b[r]);
      }

      float64x2_t sum_vec = vdupq_n_f64(0.0);
      for(int r = 0; r < UNROLL; ++r)
        sum_vec = vaddq_f64(sum_vec, sum[r]);

      C.data[i*n + j] = vgetq_lane_f64(sum_vec, 0) + vgetq_lane_f64(sum_vec, 1);
    }
  }
}

int main(){
  int n = MATRIX_SIZE;
  char filenameA[256], filenameB[256];
  snprintf(filenameA, sizeof(filenameA), "./datasets/matrix_%dx%d_A.bin", n, n);
  snprintf(filenameB, sizeof(filenameB), "./datasets/matrix_%dx%d_B.bin", n, n);
  
  FILE* fileA = fopen(filenameA, "rb");
  FILE* fileB = fopen(filenameB, "rb");

  matrix A = read_matrix(fileA);
  matrix B = read_matrix(fileB);
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
  printf("Execution time: %f seconds \n", elapsed);

  release_matrix(A);
  release_matrix(B);
  release_matrix(C);

  return 0;  
}

/*
Book example with AVX-512

#include <x86intrin.h>
#define UNROLL (4)

void dgemm(size_t n, double* A, double* B, double* C){
  for(size_t i = 0; i < n; i+=UNROLL*8){
    for(size_t j = 0; j < n; ++j){
      __m256d c[UNROLL];
      for(size_t r = 0; r < UNROLL; r++)
        c[r] = _mm512_load_pd(C+i+r*8+j*n); // [UNROLL];

      for(size_t k = 0; k < n; ++k){
        __512d bb = _mm512_broadcastsd_pd(_mm_load_sd(B+j*n+k));
        for(size_t r = 0; r < UNROLL; r++)
          c[r] = _mm512_fmadd_pd(_m512_load_pd(A+n*k+r*8+i), bb, c[r]);
      }

      for(size_t r = 0; r < UNROLL; r++)
        _mm512_store_pd(C+i+r*8+j*n, c); // [UNROLL];
    }
  }
}
*/
