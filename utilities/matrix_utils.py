from pathlib import Path
import struct

ROOT_DIR = Path(__file__).resolve().parent.parent

def read_matrix(filename): 
  matrix_path = ROOT_DIR / "datasets" / filename
  with open(matrix_path, "rb") as f:
    # read dimensions from first 8 bytes
    rows, cols = struct.unpack("<ii", f.read(8))
    matrix = []

    for _ in range(rows):
      row = []
    
      for _ in range(cols):
        value = struct.unpack("<d", f.read(8))[0]
        row.append(value)
    
      matrix.append(row)

    return matrix

def print_matrix(rows, cols, matrix):
  print(f"Rows: {rows}");
  print(f"Cols: {cols}");
  for i in range(rows):
    for j in range(cols):
      print(f"{matrix[i][j]:.6f}", end=" ")
    print("")

def compare_matrix(m1, m2, epsilon=1e-9):
  rows = len(m1)
  cols = len(m1[0])

  for i in range(rows):
    for j in range(cols):
      if abs(m1[i][j] != m2[i][j]) > epsilon:
        print(f"Mismatch at ({i}, {j})")
        print(f"Expected: {m2[i][j]}")
        print(f"Got:      {m1[i][j]}")
        return False

  return True

def zero_matrix(rows, cols):
  return [ [0.0 for _ in range(cols)] for _ in range(rows) ]
