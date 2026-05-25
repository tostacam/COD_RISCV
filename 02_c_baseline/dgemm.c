#include <stdio.h>
#include <stdlib.h>
#include "../utilities/matrix_utils.h"
#include "../utilities/benchmark.h"

#define MATRIX_SIZE 1024

typedef struct {
  matrix A;
  matrix B;
  matrix C;
} dgemm_args;

void dgemm(void* args_ptr){
  dgemm_args* args = (dgemm_args*) args_ptr;
  matrix A = args->A;
  matrix B = args->B;
  matrix C = args->C;

  int n = A.rows;
  for(int i = 0; i < n; ++i){
    for(int j = 0; j < n; ++j){
      double cij = C.data[i*n+j]; /* cij = C[i][j] */
      for(int k = 0; k < n; ++k)
        cij += A.data[i*n+k] * B.data[k*n+j];  /* cij += A[i][k] * B[k][j] */
      C.data[i*n+j] = cij; /* C[i][j] = cij */
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
 
  matrix C;
  C.rows = A.rows;
  C.cols = A.cols;
  C.data = calloc(C.rows * C.cols, sizeof(double));

  dgemm_args args = { .A = A, .B = B, .C = C };

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
