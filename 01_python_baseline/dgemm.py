from pathlib import Path
import struct

CURRENT_DIR = Path(__file__).resolve().parent
ROOT_DIR = CURRENT_DIR.parent

ROWS = COLS = 4
MATRICES = { "A", "B" }

def read_matrix(matrix_path): 
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

def dgemm(A , B, C):
  for i in range(ROWS):
    for j in range(COLS):
      for k in range(ROWS):
        C[i][j] += A[i][k] * B[k][j]

# load matrices
matrices = {}

for name in MATRICES:
  matrix_path = ROOT_DIR / "datasets" / f"matrix_{ROWS}x{COLS}_{name}.bin"
  matrices[name] = read_matrix(matrix_path)

A = matrices["A"]
B = matrices["B"]

# initialize output matrix
C = [ 
  [0.0 for _ in range(COLS)] for _ in range(ROWS)
]

# matrix multiplication
dgemm(A, B, C)

# print result
for row in C:
  print(row)
