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
        value = struct.unpack("<f", f.read(4))[0]
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
