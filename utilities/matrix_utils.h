#ifndef MATRIX_UTILS
#define MATRIX_UTILS

#include <stdio.h>

typedef struct{
  int rows;
  int cols;
  double* data;
} matrix;

matrix read_matrix(int size, FILE* file);

matrix transpose_matrix(matrix m);

void release_matrix(matrix m);

void print_matrix(matrix m);

#endif
