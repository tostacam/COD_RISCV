#include <stdio.h>
#include <stdlib.h>
#include "matrix_utils.h"

matrix read_matrix(FILE* file){
  if(file == NULL){
    printf("Failed to open file\n");
    exit(1);
  }

  matrix m;
  fread(&m.rows, sizeof(int), 1, file);
  fread(&m.cols, sizeof(int), 1, file);

  m.data = malloc(m.rows * m.cols * sizeof(double));

  if(m.data == NULL){
    printf("Allocation failed\n");
    fclose(file);
    exit(1);
  }

  size_t elements_read = fread(m.data, sizeof(double), m.rows * m.cols, file);

  if(elements_read != m.rows * m.cols){
    printf("Failed to read all elements\n");
    free(m.data);
    fclose(file);
    exit(1);
  }

  fclose(file);

  return m;
}

matrix transpose_matrix(matrix m){
  matrix t;

  t.rows = m.cols;
  t.cols = m.rows;

  t.data = malloc(t.rows * t.cols * sizeof(double));

  if(t.data == NULL){
    printf("Allocation failed");
    exit(1);
  }

  for(size_t i = 0; i < m.rows; ++i){
    for(size_t j = 0; j < m.cols; ++j){
      t.data[j * t.cols + i] = m.data[i * m.cols + j];
    }
  }

  return t;
}

void release_matrix(matrix m){
  free(m.data);
}

void print_matrix(matrix m){
  printf("Rows: %d\n", m.rows);
  printf("Cols: %d\n", m.cols);

  for(size_t i = 0; i < m.rows; ++i){
    for(size_t j = 0; j < m.cols; ++j){
      printf("%f ", m.data[i * m.cols + j]);
    }
    printf("\n");
  }
}
