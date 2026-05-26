import numpy as np

MATRIX_SIZES = [64, 128, 256, 512, 1024, 2048]

for size in MATRIX_SIZES:
  a_filename = f"datasets/matrix_{size}x{size}_A.bin"
  b_filename = f"datasets/matrix_{size}x{size}_B.bin"
  c_filename = f"datasets/matrix_{size}x{size}_C_solve.bin"

  A = np.fromfile(a_filename, dtype=np.float64).reshape(size, size)
  B = np.fromfile(b_filename, dtype=np.float64).reshape(size, size)

  C = A @ B

  C.tofile(c_filename)

  print(f"Genmerated {c_filename}")
