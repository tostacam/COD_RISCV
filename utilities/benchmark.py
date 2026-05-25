import time

def benchmark(function, *args):
  start = time.perf_counter()
  function(*args)
  end = time.perf_counter()
  elapsed = end - start
  return elapsed
