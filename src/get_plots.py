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


def show_plot(x_label, y_label, title, file_name, n_cores, threads, ff, omp, seq=None):
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

def show_plot_types(x_label, y_label, title, file_name, n_cores, floats, doubles, long_doubles):
    plt.xlabel(x_label, fontsize=30)
    plt.ylabel(y_label, fontsize=30)
    # plt.title(title, fontsize=25)
    plt.xticks(fontsize=30)
    plt.yticks(fontsize=30)
    width = 3
    plt.plot(n_cores, floats, label="float", linestyle="solid", color="#c1c0be", linewidth=width)
    plt.plot(n_cores, doubles, label="double", linestyle='dashed', color="#82817d", linewidth=width)
    plt.plot(n_cores, long_doubles, label="long double", linestyle='dotted', color="#000000", linewidth=width)
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
show_plot("Workers", "Time", "Time vs Workers", f"time_vs_cores_{file_name}", n_cores, threads_times, ff_times, omp_times, seq_times)

threads_sp = get_speedup(seq_times, threads_times)
ff_sp = get_speedup(seq_times, ff_times)
omp_sp = get_speedup(seq_times, omp_times)
show_plot("Workers", "Speedup", "Speedup vs Workers", f"speedup_vs_cores_{file_name}", n_cores, threads_sp, ff_sp, omp_sp)


threads_eff = get_efficiency(n_cores, seq_times, threads_times)
ff_eff = get_efficiency(n_cores, seq_times, ff_times)
omp_eff = get_efficiency(n_cores, seq_times, omp_times)
show_plot("Workers", "Efficiency", "Efficiency vs Workers", f"efficiency_vs_cores_{file_name}", n_cores, threads_eff, ff_eff, omp_eff)

# threads_scale = get_scaling(threads_times)
# ff_scale = get_scaling(ff_times)
# omp_scale = get_scaling(omp_times)
# show_plot("Cores", "Scaling", "Scaling vs Cores", f"scaling_vs_cores_{file_name}", n_cores, threads_scale, ff_scale, omp_scale)

data_scale = pd.read_csv("scaling.csv", sep=";")
sizes = data_scale["n"].to_numpy()
seq_times = data_scale["seq_time"].to_numpy()
threads_times = data_scale["threads_time"].to_numpy()
ff_times = data_scale["ff_time"].to_numpy()
omp_times = data_scale["omp_time"].to_numpy()
# show_plot("Size", "Time", "Time vs Size", f"time_vs_size_{file_name}", sizes, threads_times, ff_times, omp_times)
# show_plot("Size", "Time difference (wrt sequential)", "Time difference vs Size", f"difference_vs_size_{file_name}", sizes, (seq_times - threads_times)/seq_times, (seq_times - ff_times)/seq_times, (seq_times - omp_times)/seq_times)


threads_sp = get_speedup(seq_times, threads_times)
ff_sp = get_speedup(seq_times, ff_times)
omp_sp = get_speedup(seq_times, omp_times)
show_plot("Size", "Speedup", "Speedup vs Size", f"speedup_vs_size", [str(x) for x in sizes], threads_sp, ff_sp, omp_sp)


float_data = pd.read_csv("ff_results_float.csv", sep=";")
double_data = pd.read_csv("ff_results_double.csv", sep=";")
long_double_data = pd.read_csv("ff_results_long_double.csv", sep=";")

partial = pd.merge(float_data, double_data, how="inner", on="n_cores")
all_data = pd.merge(partial, long_double_data, how="inner", on="n_cores")
f_seq = all_data["seq_time_x"].to_numpy()
f_par = all_data["ff_time_x"].to_numpy()
d_seq = all_data["seq_time_y"].to_numpy()
d_par = all_data["ff_time_y"].to_numpy()
ld_seq = all_data["seq_time"].to_numpy()
ld_par = all_data["ff_time"].to_numpy()

f_sp = get_speedup(f_seq, f_par)
d_sp = get_speedup(d_seq, d_par)
ld_sp = get_speedup(ld_seq, ld_par)
show_plot_types("Workers", "Speedup", "Speedup vs Workers", f"data_types_speedup", n_cores, f_sp, d_sp, ld_sp)

