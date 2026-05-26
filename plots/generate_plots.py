import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

# Fetching results from csv's
results_dir = Path("results");
dfs = []
for csv_file in results_dir.glob("*.csv"):
  df = pd.read_csv(csv_file)
  dfs.append(df)
results = pd.concat(dfs, ignore_index=True)

# Speedup progression for 2048x2048
TARGET_SIZE = 2048

ordered_impls = [
  ("01_python_baseline", "Python (Ch. 1)"),
  ("02_c_baseline", "Translate to C (Ch. 2)"),
  ("03_SIMD", "Data Level Parallelism (Ch. 3)"),
  ("04_ILP", "Instruction Level Parallelism (Ch. 4)"),
  ("05_cache_blocking", "Cache Blocking (Ch. 5)"),
  ("06_multicore", "Thread Level Parallelism (Ch. 6)")
]

baseline_time = results[
  (results["implementation"] == "01_python_baseline") &
  (results["matrix_size"] == TARGET_SIZE)
]["time_sec"].values[0]

speedups = []
labels = []
for impl_id, label in ordered_impls:
  impl_rows = results[
    (results["implementation"] == impl_id) &
    (results["matrix_size"] == TARGET_SIZE)
  ]
  
  impl_time = impl_rows["time_sec"].values[0];
  speedups.append(baseline_time / impl_time)
  labels.append(label)

plt.figure(figsize=(10, 8))
plt.plot(labels, speedups)
for i, value in enumerate(speedups):
  plt.text(i, value * 1.2, f"{value:.1f}", ha="center")
plt.yscale("log")

plt.ylabel("Speedup")
plt.title("Optimization of Matrix Multiply (2048x2048)")
plt.grid(True, axis="y", which="both")
plt.xticks(rotation=25, ha="right")
plt.subplots_adjust(bottom=0.22)
plt.savefig("./plots/speedup_plot.png", dpi=600, bbox_inches="tight")
#plt.show()

# GFLOPS/s per each stage
target_impls = [
  ("01_python_baseline", "Python"),
  ("02_c_baseline", "C"),
  ("03_SIMD", "+SIMD +O3"),
  ("04_ILP", "+Unrolled"),
  ("05_cache_blocking", "+Blocked"),
  ("06_multicore", "+Threads")
]

matrix_sizes = sorted(results["matrix_size"].unique())
colors = plt.cm.Blues(
  [0.35 + (0.5 * i / (len(matrix_sizes) - 1)) for i in range(len(matrix_sizes))]
)
bar_width = 0.12
x = range(len(target_impls))
plt.figure(figsize=(10, 5))
for i, (size, color) in enumerate(zip(matrix_sizes, colors)):
  gflops_values = []

  for impl_id, _ in target_impls:
    row = results[
      (results["implementation"] == impl_id) &
      (results["matrix_size"] == size)
    ]

    if row.empty:
      gflops_values.append(0)
    else:
      gflops_values.append(row["gflops"].values[0])

  positions = [
    p + i * bar_width
    for p in x
  ]

  bars = plt.bar(
    positions,
    gflops_values,
    width=bar_width,
    label=f"{size}x{size}",
    color=color
  )
  for bar, value in zip(bars, gflops_values):
    plt.text(
      bar.get_x() + bar.get_width() / 2,
      value + 1,
      f"{value:.1f}",
      ha="center",
      va="bottom",
      fontsize=7,
      rotation=90
    )

center_positions = [
  p + ((len(matrix_sizes) - 1) * bar_width) / 2
  for p in x
]
plot_labels = [label for _, label in target_impls]
plt.xticks(
    center_positions,
    plot_labels
)

plt.ylabel("GFLOPS/Second")
plt.grid(True, axis="y")
plt.legend(title="Matrix Size")
plt.subplots_adjust(bottom=0.15)
plt.savefig("./plots/gflops_plot.png", dpi=600, bbox_inches="tight")
#plt.show()
