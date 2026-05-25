#ifndef MATRIX_UTILS
#define MATRIX_UTILS

#include <stdio.h>

typedef struct{
  int rows;
  int cols;
  float* data;
} matrix;

matrix read_matrix(FILE* file);

void release_matrix(matrix m);

void print_matrix(matrix m);

#endif
