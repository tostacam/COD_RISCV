import csv
from utilities.matrix_utils import read_matrix, zero_matrix, compare_matrix, print_matrix
from utilities.benchmark import benchmark
from .dgemm import dgemm

MATRIX_SIZES = [64, 128, 256, 512, 1024, 2048]
RESULTS_FILE = "results/01_python_baseline.csv"

def compute_gflops(n, elapsed):
  flops = 2 * (n ** 3)
  return flops / (elapsed * 1e9)

with open(RESULTS_FILE, "w", newline="") as file:
  writer = csv.writer(file)

  writer.writerow([
    "implementation",
    "matrix_size",
    "threads",
    "time_sec",
    "gflops",
    "validated",
  ])

for size in MATRIX_SIZES:
  print(f"Running {size}x{size}")

  A = read_matrix(size, f"matrix_{size}x{size}_A.bin")
  B = read_matrix(size, f"matrix_{size}x{size}_B.bin")
  C_ref = read_matrix(size, f"matrix_{size}x{size}_C_solve.bin")
  C = zero_matrix(size, size)

  elapsed = benchmark(dgemm, A, B, C, size, size)
  validated = compare_matrix(C, C_ref)
  gflops = compute_gflops(size, elapsed)

  print(f"Time: {elapsed:.4f}s")
  print(f"GFLOPS: {gflops:.4f}")
  print(f"Validated: {validated}")
  print()

  with open(RESULTS_FILE, "a", newline="") as file:
    writer = csv.writer(file)

    writer.writerow([
      "01_python_baseline",
      size,
      1,
      elapsed,
      gflops,
      validated
    ])
