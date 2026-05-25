from pathlib import Path
import struct
import random

DATASET_DIR = Path(__file__).resolve().parent

ROWS = 4
COLS = 4

SEEDS = {
  "A": 123,
  "B": 456,
  "C": 789
}

def generate_matrix(filename, seed):
  random.seed(seed)

  matrix = [
    [random.random() for _ in range(COLS)] for _ in range(ROWS)    
  ]

  filepath = DATASET_DIR / filename
  with open(filepath, "wb") as f:
    # write dimensions on first 8 bytes
    f.write(struct.pack("<ii", ROWS, COLS))

    # write matrix data
    for row in matrix:
      for value in row:
        f.write(struct.pack("<f", value))

  print(f"Generated {filename}")

# generate all matrices
for name, seed in SEEDS.items():
  filename = f"matrix_{ROWS}x{COLS}_{name}.bin"
  generate_matrix(filename, seed)
