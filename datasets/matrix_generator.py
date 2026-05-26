from pathlib import Path
import struct
import random

DATASET_DIR = Path(__file__).resolve().parent

MATRIX_SIZES = [64, 128, 256, 512, 1024, 2048]

SEEDS = {
  "A": 1234,
  "B": 5678,
}

def generate_matrix(filename, seed, size):
  random.seed(seed)

  matrix = [
    [random.random() for _ in range(size)] for _ in range(size)
  ]

  filepath = DATASET_DIR / filename
  with open(filepath, "wb") as f:
    # write dimensions on first 8 bytes
    f.write(struct.pack("<ii", size, size))

    # write matrix data
    for row in matrix:
      for value in row:
        f.write(struct.pack("<d", value))

  print(f"Generated {filename}")

# generate all matrices
for size in MATRIX_SIZES:
  for name, seed in SEEDS.items():
    filename = f"matrix_{size}x{size}_{name}.bin"
    generate_matrix(filename, seed, size)
