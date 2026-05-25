import struct
import random

# set seed
# A -> 123
# B -> 456
# C -> 789
random.seed(123)

# matrix size
rows = 64
cols = 64

matrix = [
  [random.random() for _ in range(cols)]
  for _ in range(rows)
]

with open("matrix_64x64_B.bin", "wb") as f:
  # write dimensions
  f.write(struct.pack(">ii", rows, cols))

  # write matrix data
  for row in matrix:
    for value in row:
      f.write(struct.pack("f", value))
