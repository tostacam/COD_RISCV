#include <stdio.h>
#include <stdlib.h>

#define MATRIX_SIZE 4

typedef struct {
  int rows;
  int cols;
  float* data;
} matrix;

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
matrix read_matrix(FILE* file){
  if(file == NULL){
    printf("Failed to open file\n");
    exit(1);
  }

  matrix m;
  fread(&m.rows, sizeof(int), 1, file);
  fread(&m.cols, sizeof(int), 1, file);

  m.data = malloc(m.rows * m.cols * sizeof(float));

  if(m.data == NULL){
    printf("Allocation failed\n");
    fclose(file);
    exit(1);
  }

  size_t elements_read = fread(m.data, sizeof(float), m.rows * m.cols, file);

  if(elements_read != m.rows * m.cols){
    printf("Failed to read all elements\n");
    free(m.data);
    fclose(file);
    exit(1);
  }

  fclose(file);

  return m;
}

void release_matrix(matrix m){
  free(m.data);
}

void print_matrix(matrix m){
  printf("Rows: %d\n", m.rows);
  printf("Cols: %d\n", m.cols);
  
  for(int i = 0; i < m.rows; ++i){
    for(int j = 0; j < m.cols; ++j){
      printf("%f ", m.data[i * m.cols + j]);
    }
    printf("\n");
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
