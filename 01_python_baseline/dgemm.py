from utilities.matrix_io import read_matrix
from utilities.matrix_utils import zero_matrix
from utilities.benchmark import benchmark

ROWS = COLS = 1024

def dgemm(A , B, C):
  for i in range(ROWS):
    for j in range(COLS):
      for k in range(ROWS):
        C[i][j] += A[i][k] * B[k][j]

A = read_matrix(f"matrix_{ROWS}x{COLS}_A.bin")
B = read_matrix(f"matrix_{ROWS}x{COLS}_B.bin")
C = zero_matrix(ROWS, COLS)

elapsed = benchmark(dgemm, A, B, C)

print(f"Execution time: {elapsed:.6f} seconds")
