from cProfile import label
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("results.csv", sep=";")
# print(data["n_cores"])
n_cores = data["n_cores"].to_numpy()
threads_sp = data["threads_sp"].to_numpy()
ff_sp = data["ff_sp"].to_numpy()
omp_sp = data["omp_sp"].to_numpy()

plt.plot(n_cores, threads_sp, label="C++ threads")
plt.plot(n_cores, ff_sp, label="Fast Flow")
plt.plot(n_cores, omp_sp, label="Open MP")
plt.legend()
# plt.xticks(n_cores)
plt.show()

# plt.plot()