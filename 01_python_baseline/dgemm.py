def dgemm(A, B, C, rows, cols):
  for i in range(rows):
    for j in range(cols):
      for k in range(rows):
        C[i][j] += A[i][k] * B[k][j]
