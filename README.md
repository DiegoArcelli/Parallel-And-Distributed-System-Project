# Parallel-And-Distributed-Systems-Project
Parallel implementation of the Jacobi iterative method to solve systems of linear equations, for my parallel and distributed systems exam.

## Implementations
Besides the straightforward sequential implementation, there are three different parallel implementations:
- C++ threads
- FastFlow
- OpenMP

## Compile and run
To compile the code is sufficient to run the command `make` in the `src` direcory of the repository. The compilation will produce an executable called `main` which will require the following parameters:
- n: the size of the matrix
- nw: the number of thread that will be used in the parallel implementation
- iters: the number of iterations of the algorithm
- mode: specify which implementations will be executed

The possible values for the mode parameter are:
- 0: execute all the implementations
- 1: execute the sequential implementation only
- 2: execute all the parallel implementations
- 3: execute the C++ threads implementations only
- 4: execute the FastFlow implementation only
- 5: execute the OpenMP implementation only

For example with the following command:
```
./main 1000 4 100 0
```
we will run the all the implementations of the algorithm on 1000 $\times$ 1000 matrix for 100 iterations, using 4 threads for the parallel implementations.

## Run the experiments
There are two bash shell scripts which can be used to collect data for the experiments. The first script is `get_times.sh`, and it takes the times of all the implementations, for a different amount of threads used. The script requires three arguments:
- n: the size of the matrix
- n_cores: the maximum number of threads to use  
- iters: the number of iterations of the algorithm
For instance if we run the command:
```
./get_times.sh 1000 16 100
```
the script will record the exuction times running the program on a 1000 $\times$ 1000 matrix for 100 iterations, first running the parallel versions with 1 thread, then with 2, then with 3, and so on up until 16 threads.
The results of the experiments are by default written in a file called `results.csv`.

The second script is `get_scaling.sh` and it run the experiments for a fixed amount of cores, of matrices of different sizes, to check how the parallel implementations scale with respect to the size of the matrix. The script requires two arguments:
- n_cores: the number of cores to use in the experiment
- iters: the number of iterations of the algorithm

For example if we run the command:
```
./get_scaling.sh 16 100
```
The script will run all the implementations using 16 cores, fo 100 iterations, first on a $10\times10$ matrix, then on a $100\times100$, then on a $500\times500$ matrix, and so on. The results of the experiments are by default written in a file called `results_scaling.csv`.

To get the plots of execution times, speedup and effeciency, you can run the python script `get_plots.py`, which requires two arguments:
- `--show` or `-s` to show the plots on the screen
- `--output` or `-o` to set the common prefix of the file names of the plots