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
#include <omp.h>
#define UNROLL 2
#define BLOCKSIZE 256

void do_block(int n, int si, int sj, int sk, double* A, double* BT, double* C){
  int i_end = (si + BLOCKSIZE < n) ? si + BLOCKSIZE : n;
  int j_end = (sj + BLOCKSIZE < n) ? sj + BLOCKSIZE : n;
  int k_end = (sk + BLOCKSIZE < n) ? sk + BLOCKSIZE : n;

  for(size_t i = si; i < i_end; i += 2){
    for(size_t j = sj; j < j_end; j += 2){
      float64x2_t sum00[UNROLL];
      float64x2_t sum01[UNROLL];
      float64x2_t sum10[UNROLL];
      float64x2_t sum11[UNROLL];

      for(int r = 0; r < UNROLL; ++r){
        sum00[r] = vdupq_n_f64(0.0);
        sum01[r] = vdupq_n_f64(0.0);
        sum10[r] = vdupq_n_f64(0.0);
        sum11[r] = vdupq_n_f64(0.0);
      }

      float64x2_t a0[UNROLL];
      float64x2_t a1[UNROLL];
      float64x2_t b0[UNROLL];
      float64x2_t b1[UNROLL];

      for(size_t k = sk; k < k_end; k += 2 * UNROLL){
        for(int r = 0; r < UNROLL; ++r){
          a0[r] = vld1q_f64(&A[i*n + k + r*2]);
          b0[r] = vld1q_f64(&BT[j*n + k + r*2]);

          if(i + 1 < i_end)
            a1[r] = vld1q_f64(&A[(i+1)*n + k + r*2]);

          if(j + 1 < j_end)
            b1[r] = vld1q_f64(&BT[(j+1)*n + k + r*2]);
        }

        for(int r = 0; r < UNROLL; ++r){
          sum00[r] = vfmaq_f64(sum00[r], a0[r], b0[r]);

          if(j + 1 < j_end)
            sum01[r] = vfmaq_f64(sum01[r], a0[r], b1[r]);

          if(i + 1 < i_end)
            sum10[r] = vfmaq_f64(sum10[r], a1[r], b0[r]);

          if(i + 1 < i_end && j + 1 < j_end)
            sum11[r] = vfmaq_f64(sum11[r], a1[r], b1[r]);
        }
      }

      float64x2_t sum_vec00 = vdupq_n_f64(0.0);
      float64x2_t sum_vec01 = vdupq_n_f64(0.0);
      float64x2_t sum_vec10 = vdupq_n_f64(0.0);
      float64x2_t sum_vec11 = vdupq_n_f64(0.0);

      for(int r = 0; r < UNROLL; ++r){
        sum_vec00 = vaddq_f64(sum_vec00, sum00[r]);
        sum_vec01 = vaddq_f64(sum_vec01, sum01[r]);
        sum_vec10 = vaddq_f64(sum_vec10, sum10[r]);
        sum_vec11 = vaddq_f64(sum_vec11, sum11[r]);
      }

      C[i*n + j] += vgetq_lane_f64(sum_vec00, 0) + vgetq_lane_f64(sum_vec00, 1);

      if(j + 1 < j_end)
        C[i*n + j + 1] += vgetq_lane_f64(sum_vec01, 0) + vgetq_lane_f64(sum_vec01, 1);

      if(i + 1 < i_end)
        C[(i+1)*n + j] += vgetq_lane_f64(sum_vec10, 0) + vgetq_lane_f64(sum_vec10, 1);

      if(i + 1 < i_end && j + 1 < j_end)
        C[(i+1)*n + j + 1] += vgetq_lane_f64(sum_vec11, 0) + vgetq_lane_f64(sum_vec11, 1);
    }
  }
}

void dgemm(void* args_ptr){
  dgemm_args* args = (dgemm_args*) args_ptr;
  matrix A = args->A;
  matrix BT = args->BT;
  matrix C = args->C;

  size_t n = A.rows;
  #pragma omp parallel for collapse(2)
  for(int si = 0; si < n; si += BLOCKSIZE)
    for(int sj = 0; sj < n; sj += BLOCKSIZE)
      for(int sk = 0; sk < n; sk += BLOCKSIZE)
        do_block(n, si, sj, sk, A.data, BT.data, C.data);
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
#include <x86intrin.h>
#define UNROLL (4)
#define BLOCKSIZE 32

void do_block(int n, int si, int sj, int sk, double* A, double* B, double* C){
  for(int i = si; i < si + BLOCKSIZE; i+=UNROLL*8){
    for(int j = sj; i < sj + BLOCKSIZE; j++){
      __m512d c[UNROLL];
      for(int r = 0; r < UNROLL; r++)
        c[r] = _mm512_load_pd(C+i+r*8+j*n); // [UNROLL];
      
      for(int k = sk; k < sk + BLOCKSIZE; k++){
        __m512d bb = _mm512_broadcast_pd(_mm_load_sd(B+j*n+k));
        for(int r = 0; r < UNROLL, r++)
          c[r] = _mm512_fmadd_pd(_mm512_load_pd(A+n*k+r*8+i), bb, c[r]);
      }

      for(int r = 0; r < UNROLL; r++)
        _mm512_store_pd(C+i+r*8+j*n, c[r]);
    }
  }
}

void dgemm(size_t n, double* A, double* B, double* C){
#pragma omp parallel for
  for(int sj = 0; sj < n; sj += BLOCKSIZE)
    for(int si = 0; si < n; si += BLOCKSIZE)
      for(int sk = 0; sk < n; sk += BLOCKSIZE)
        do_block(n, si, sj, sk, A, B, C);
}
*/
