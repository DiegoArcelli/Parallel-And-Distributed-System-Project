import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import argparse


def get_speedup(seq_times, par_times):
    speedups = []
    for i in range(len(par_times)):
        speedups.append(seq_times[i]/par_times[i])
    return np.array(speedups)


def get_efficiency(n_cores, seq_times, par_times):
    speedups = get_speedup(seq_times, par_times)
    return np.array([speedups[i]/n_cores[i] for i in range(len(speedups))])


def get_scaling(par_times):
    return np.array([par_times[0]/par_times[i] for i in range(len(par_times))])


def show_plot(x_label, y_label, title, file_name, n_cores, threads, ff, omp, seq=None):
    plt.xlabel(x_label)
    plt.ylabel(y_label)
    plt.title(title)
    plt.plot(n_cores, threads, label="C++ threads", linestyle="solid", color="grey")
    plt.plot(n_cores, ff, label="Fast Flow", linestyle='dashed', color="gray")
    plt.plot(n_cores, omp, label="Open MP", linestyle='dotted', color="gray")
    if seq is not None:
        plt.plot(n_cores, seq, label="Sequential", linestyle="dashdot", color="gray")
    plt.legend()
    plt.savefig(f"./plots/{file_name}.png")
    print(f"Saved: ./plots/{file_name}.png")
    if show_flag:
        plt.show()
    plt.close()


parser = argparse.ArgumentParser()
parser.add_argument("--show", "-s", action=argparse.BooleanOptionalAction)
parser.add_argument("--output", "-o", type=str, default="experiment")
args = parser.parse_args()
args = vars(args)
show_flag = args["show"]
file_name = args["output"] 


# plotting execution time
data_cores = pd.read_csv("results_16384.csv", sep=";")
n_cores = data_cores["n_cores"].to_numpy()
seq_times = data_cores["seq_time"].to_numpy()
threads_times = data_cores["threads_time"].to_numpy()
ff_times = data_cores["ff_time"].to_numpy()
omp_times = data_cores["omp_time"].to_numpy()
show_plot("Cores", "Time", "Time vs Cores", f"time_vs_cores_{file_name}", n_cores, threads_times, ff_times, omp_times, seq_times)

threads_sp = get_speedup(seq_times, threads_times)
ff_sp = get_speedup(seq_times, ff_times)
omp_sp = get_speedup(seq_times, omp_times)
if show_flag:
    show_plot("Cores", "Speedup", "Speedup vs Cores", f"speedup_vs_cores_{file_name}", n_cores, threads_sp, ff_sp, omp_sp)


threads_eff = get_efficiency(n_cores, seq_times, threads_times)
ff_eff = get_efficiency(n_cores, seq_times, ff_times)
omp_eff = get_efficiency(n_cores, seq_times, omp_times)
show_plot("Cores", "Efficiency", "Efficiency vs Cores", f"efficiency_vs_cores_{file_name}", n_cores, threads_eff, ff_eff, omp_eff)

threads_scale = get_scaling(threads_times)
ff_scale = get_scaling(ff_times)
omp_scale = get_scaling(omp_times)
show_plot("Cores", "Scaling", "Scaling vs Cores", f"scaling_vs_cores_{file_name}", n_cores, threads_scale, ff_scale, omp_scale)

data_scale = pd.read_csv("scaling.csv", sep=";")
sizes = data_scale["n"].to_numpy()
seq_times = data_scale["seq_time"].to_numpy()
threads_times = data_scale["threads_time"].to_numpy()
ff_times = data_scale["ff_time"].to_numpy()
omp_times = data_scale["omp_time"].to_numpy()
show_plot("Size", "Time", "Time vs Size", f"time_vs_size_{file_name}", sizes, threads_times, ff_times, omp_times, seq_times)



threads_sp = get_speedup(seq_times, threads_times)
ff_sp = get_speedup(seq_times, ff_times)
omp_sp = get_speedup(seq_times, omp_times)
show_plot("Size", "Speedup", "Speedup vs Size", f"speedup_vs_size_{file_name}", sizes, threads_sp, ff_sp, omp_sp)
