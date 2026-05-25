#include <stdio.h>
#include <stdlib.h>
#include "../utilities/matrix_utils.h"

#define MATRIX_SIZE 4

void dgemm(matrix A, matrix B, matrix C){
  int n = A.rows;
  for(int i = 0; i < n; ++i){
    for(int j = 0; j < n; ++j){
      float cij = C.data[i+j*n]; /* cij = C[i][j] */
      for(int k = 0; k < n; ++k)
        cij = A.data[i+k*n] * B.data[k+j*n];  /* cij += A[i][k] * B[k][j] */
      C.data[i+j*n] = cij; /* C[i][z] = cij */
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
  C.data = calloc(C.rows * C.cols, sizeof(float));

  dgemm(A, B, C);

  // print_matrix(C);

  release_matrix(A);
  release_matrix(B);
  release_matrix(C);

  return 0;  
}
