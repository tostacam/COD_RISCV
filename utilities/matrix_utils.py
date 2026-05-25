def zero_matrix(rows, cols):
  return [ [0.0 for _ in range(cols)] for _ in range(rows) ]

def print_matrix(matrix):
  for row in matrix:
    print(row)
