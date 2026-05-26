#include <stdio.h>
#include <stdlib.h>
#include "../utilities/matrix_utils.h"
#include "../utilities/benchmark.h"

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
 
  matrix C;
  C.rows = A.rows;
  C.cols = A.cols;
  C.data = calloc(C.rows * C.cols, sizeof(double));

  dgemm_args args = { .A = A, .B = B, .C = C };

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
