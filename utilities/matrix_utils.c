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
