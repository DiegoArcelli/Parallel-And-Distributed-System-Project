from functools import partial
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


def show_plot(x_label, y_label, file_name, n_cores, threads, ff, omp, seq=None):
    plt.xlabel(x_label, fontsize=30)
    plt.ylabel(y_label, fontsize=30)
    # plt.title(title, fontsize=25)
    plt.xticks(fontsize=30)
    plt.yticks(fontsize=30)
    width = 3
    plt.plot(n_cores, threads, label="C++ threads", linestyle="solid", color="#c1c0be", linewidth=width)
    plt.plot(n_cores, ff, label="Fast Flow", linestyle='dashed', color="#82817d", linewidth=width)
    plt.plot(n_cores, omp, label="Open MP", linestyle='dotted', color="#000000", linewidth=width)
    if seq is not None:
        plt.plot(n_cores, seq, label="Sequential", linestyle="dashdot", color="gray", linewidth=width)
    plt.legend(fontsize=30)
    plt.savefig(f"./plots/{file_name}.png", dpi=100, bbox_inches='tight')
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
# plt.rcParams["figure.figsize"] = (20,15)
plt.rcParams["figure.figsize"] = (20,10)



# plotting execution time
data_cores = pd.read_csv(f"results_{file_name}.csv", sep=";")
n_cores = data_cores["n_cores"].to_numpy()
seq_times = data_cores["seq_time"].to_numpy()
threads_times = data_cores["threads_time"].to_numpy()
ff_times = data_cores["ff_time"].to_numpy()
omp_times = data_cores["omp_time"].to_numpy()
show_plot("Workers", "Time (nanoseconds)", f"time_vs_cores_{file_name}", n_cores, threads_times, ff_times, omp_times, seq_times)

threads_sp = get_speedup(seq_times, threads_times)
ff_sp = get_speedup(seq_times, ff_times)
omp_sp = get_speedup(seq_times, omp_times)
show_plot("Workers", "Speedup", f"speedup_vs_cores_{file_name}", n_cores, threads_sp, ff_sp, omp_sp)


threads_eff = get_efficiency(n_cores, seq_times, threads_times)
ff_eff = get_efficiency(n_cores, seq_times, ff_times)
omp_eff = get_efficiency(n_cores, seq_times, omp_times)
show_plot("Workers", "Efficiency", f"efficiency_vs_cores_{file_name}", n_cores, threads_eff, ff_eff, omp_eff)

data_scale = pd.read_csv("scaling_16.csv", sep=";")
sizes = data_scale["n"].to_numpy()
seq_times = data_scale["seq_time"].to_numpy()
threads_times = data_scale["threads_time"].to_numpy()
ff_times = data_scale["ff_time"].to_numpy()
omp_times = data_scale["omp_time"].to_numpy()

threads_sp = get_speedup(seq_times, threads_times)
ff_sp = get_speedup(seq_times, ff_times)
omp_sp = get_speedup(seq_times, omp_times)
show_plot("Size", "Speedup", f"speedup_vs_size", [str(x) for x in sizes], threads_sp, ff_sp, omp_sp)