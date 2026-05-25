from utilities.matrix_io import read_matrix
from utilities.matrix_utils import zero_matrix

ROWS = COLS = 4

def dgemm(A , B, C):
  for i in range(ROWS):
    for j in range(COLS):
      for k in range(ROWS):
        C[i][j] += A[i][k] * B[k][j]

A = read_matrix(f"matrix_{ROWS}x{COLS}_A.bin")
B = read_matrix(f"matrix_{ROWS}x{COLS}_B.bin")
C = zero_matrix(ROWS, COLS)

# matrix multiplication
dgemm(A, B, C)
