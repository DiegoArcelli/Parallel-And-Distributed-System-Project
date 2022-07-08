#include "Jacobi.hpp"
#include "timer.hpp"
#include <ctime>
#include <vector>
#include <functional>
#include <cstdlib>
#include <thread>
#include <iostream>
#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>
#include <barrier>
#include <omp.h>

using namespace ff;

Jacobi::Jacobi(int n) {
    this->n = n;
    A = generate_diagonal_dominant_matrix(n);
    b = generate_random_vector(n);
}


double* Jacobi::sequential(int iterations) {
    double* x = new double[n];
    for (int i = 0; i < n; i++) x[i] = 0;// rand() % 10; 
    
    {
        timer t("seq");
        double* x_aux = new double[n];
        double* temp;
        for (int k = 0; k < iterations; k++) {
            for (int i = 0; i < n; i++) {
                x_aux[i] = 0;
                for (int j = 0; j < n; j++) {
                    if (i != j) {
                        x_aux[i] -= A[i][j]*x[j];
                    }
                }
                x_aux[i] = (x_aux[i] + b[i])/A[i][i];
            }
            temp = x_aux;
            x_aux = x;
            x = temp;
        }
        delete x_aux;
    }

    return x;
}

double* Jacobi::parallel(std::string mode, int iterations, int nw) {
    
    if (mode == "cpp") {
        return this->parallel_threads(iterations, nw);
    } else if (mode == "ff") { 
        return this->parallel_ff(iterations, nw);
    } else if (mode == "omp") {
        return this->parallel_omp(iterations, nw);
    }

    return nullptr;
}

double* Jacobi::parallel_threads(int iterations, int nw) {

    double* x = new double[n];
    for (int i = 0; i < n; i++) x[i] = 0;// rand() % 10;
    

    {
        timer t("cpp");
        double* x_aux= new double[n]; 
        double* temp;
        int k = 0;
        int start, end;
        std::barrier copy_barrier(nw, []() { return; });
        std::barrier next_iteration_barrier(nw, []() { return; });
        std::thread tids[nw];

        std::function<void(int, int)> thread_map_task = [&](int start, int end){

            int i, j;
            while (k < iterations) {

                for (i = start; i <= end; i++) {
                    x_aux[i] = 0;
                    for (int j = 0; j < n; j++) {
                        if (i != j) {
                            x_aux[i] -= A[i][j]*x[j];
                        }
                    }
                    x_aux[i] = (x_aux[i] + b[i])/A[i][i];
                }

                copy_barrier.arrive_and_wait();

                if (start == 0) {
                    temp = x_aux;
                    x_aux = x;
                    x = temp;
                    k+=1;
                }

                next_iteration_barrier.arrive_and_wait();
            }

        };

        int size = n/nw;
        for (int i = 0; i < nw; i++) {
            start = i*size;
            end = (i != nw-1 ? start + size - 1 : n-1);
            tids[i] = std::thread(thread_map_task, start, end);
        }

        for (int i = 0; i < nw; i++) {
            tids[i].join();
        }
        delete x_aux;
    }

    return x;

}

double* Jacobi::parallel_ff(int iterations, int nw) {
    
    double* x = new double[n];
    for (int i = 0; i < n; i++) x[i] = 0;// rand() % 10;

    {
        timer t("fff");
        double* x_aux = new double[n]; 
        double* temp;
        ParallelFor pf(nw);
        int size = n/nw;
        int k = 0;
        int j;

        while (k < iterations) {
            pf.parallel_for(0, n, 1, size, [&](const long i) {
                x_aux[i] = 0;
                for (int j = 0; j < n; j++) {
                    if (i != j) {
                        x_aux[i] -= A[i][j]*x[j];
                    }
                }
                x_aux[i] = (x_aux[i] + b[i])/A[i][i];
            }, nw);
                
            temp = x_aux;
            x_aux = x;
            x = temp;
            k+=1;

        }
        delete x_aux;
    }

    return x;

}

double* Jacobi::parallel_omp(int iterations, int nw) {

    double* x = new double[n];
    for (int i = 0; i < n; i++) x[i] = 0;

    {
        timer t("omp");
        double* x_aux = new double[n]; 
        double* temp;
        int i, j;
        int k = 0;
        while (k < iterations) {
            #pragma omp parallel for schedule(static) private(i,j) num_threads(nw)
            for (i = 0; i < n; i++) {
                x_aux[i] = 0;
                for (j = 0; j < n; j++) {
                    if (i != j) {
                        x_aux[i] -= A[i][j]*x[j];
                    }
                }
                x_aux[i] = (x_aux[i] + b[i])/A[i][i];
            }
            
            temp = x_aux;
            x_aux = x;
            x = temp;   
            k+=1;

        }
        delete x_aux;
    }
    return x;

}