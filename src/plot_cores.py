import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


def get_speedup(seq_times, par_times):
    speedups = []
    for i in range(len(par_times)):
        speedups.append(seq_times[i]/par_times[i])
    return np.array(speedups)


def get_efficiency(n_cores, seq_times, par_times):
    speedups = get_speedup(seq_times, par_times)
    return np.array([speedups[i]/n_cores[i] for i in range(len(speedups))])


def get_scaling(par_times):
    return np.array([par_times[i]/par_times[0] for i in range(len(par_times))])


def show_plot(n_cores, threads, ff, omp):
    plt.plot(n_cores, threads, label="C++ threads")
    plt.plot(n_cores, ff, label="Fast Flow", linestyle='dashed')
    plt.plot(n_cores, omp, label="Open MP", linestyle='dotted')
    plt.legend()
    plt.show()


data_cores = pd.read_csv("results.csv", sep=";")

n_cores = data_cores["n_cores"].to_numpy()
seq_times = data_cores["seq_time"].to_numpy()
threads_times = data_cores["threads_time"].to_numpy()
ff_times = data_cores["ff_time"].to_numpy()
omp_times = data_cores["omp_time"].to_numpy()

threads_sp = get_speedup(seq_times, threads_times)
ff_sp = get_speedup(seq_times, ff_times)
omp_sp = get_speedup(seq_times, omp_times)
show_plot(n_cores, threads_sp, ff_sp, omp_sp)


# threads_eff = get_efficiency(n_cores, seq_times, threads_times)
# ff_eff = get_efficiency(n_cores, seq_times, ff_times)
# omp_eff = get_efficiency(n_cores, seq_times, omp_times)
# show_plot(n_cores, threads_eff, ff_eff, omp_eff)

# threads_scale = get_scaling(threads_times)
# ff_scale = get_scaling(ff_times)
# omp_scale = get_scaling(omp_times)
# show_plot(n_cores, threads_scale, ff_scale, omp_scale)


# data_scale = pd.read_csv("scaling.csv", sep=";")
# dims = data_scale["n"].to_numpy()
# seq_times = data_scale["seq_time"].to_numpy()
# threads_times = data_scale["threads_time"].to_numpy()
# ff_times = data_scale["ff_time"].to_numpy()
# omp_times = data_scale["omp_time"].to_numpy()

# threads_sp = get_speedup(seq_times, threads_times)
# ff_sp = get_speedup(seq_times, ff_times)
# omp_sp = get_speedup(seq_times, omp_times)
# show_plot(dims, threads_sp, ff_sp, omp_sp)