#include "Jacobi.hpp"
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

#define DEBUG(i) (std::cout << "ZIOPERA" << i << "\n")

Jacobi::Jacobi(int n) {
    this->n = n;
    A = new real*[n];
    b = new real[n];
    for (int i = 0; i < n; i++) {
        A[i] = new real[n];
        for (int j = 0; j < n; j++) {
            A[i][j] = rand() % 10;
        }
        if (A[i][i] == 0) A[i][i] = (rand() % 9) + 1;
        b[i] = rand() % 10;
    }
}


real* Jacobi::sequential(int iterations) {
    real* x = new real[n];
    srand(time(NULL));
    for (int i = 0; i < n; i++) x[i] = 0;// rand() % 10;
    real* x_aux = new real[n]; 
    real val;
    real* temp;
    for (int k = 0; k < iterations; k++) {
        for (int i = 0; i < n; i++) {
            val = 0;
            for (int j = 0; j < n; j++) {
                val += (j != i ? A[i][j]*x[j] : 0);
            }
            x_aux[i] = (1/A[i][i])/(b[i] - val);
        }
        temp = x_aux;
        x_aux = x;
        x = temp;
        // for (int i = 0; i < n; i++)
        //     x[i] = x_aux[i];
    }
    delete x_aux;
    return x;
}

real* Jacobi::parallel(std::string mode, int iterations, int nw1, int nw2) {
    
    if (mode == "threads") {
        return this->parallel_threads(iterations, nw1, nw2);
    } else if (mode == "ff") { 
        return this->parallel_ff(iterations, nw1, nw2);
    } else if (mode == "omp") {
        return this->parallel_omp(iterations, nw1, nw2);
    }

    return nullptr;
}

real* Jacobi::parallel_threads(int iterations, int nw1, int nw2) {

    int k = 0;
    srand(time(NULL));
    real* x = new real[n];
    for (int i = 0; i < n; i++) x[i] = 0;// rand() % 10;
    real* x_aux= new real[n]; 
    real* temp;
    int nw = nw1*nw2;
    std::barrier copy_barrier(nw1, []() { return; });
    std::barrier next_iteration_barrier(nw1, []() { return; });
    std::thread tids[nw1];

    std::function<void(int, int)> thread_map_task = [&](int start, int end){

        while (k < iterations) {

            // do the first map calling the reduce operations
            for (int i = start; i <= end; i++) {
                real val = 0;
                for (int j = 0; j < n; j++) {
                    val += (j != i ? A[i][j]*x[j] : 0);
                }
                x_aux[i] = (1/A[i][i])/(b[i] - val);
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

    int size = n/nw1;
    for (int i = 0; i < nw1; i++) {
        int start = i*size;
        int end = (i != nw1-1 ? start + size - 1 : n-1);
        tids[i] = std::thread(thread_map_task, start, end);
    }

    for (int i = 0; i < nw1; i++) {
        tids[i].join();
    }

    delete x_aux;

    return x;

}

real* Jacobi::parallel_ff(int iterations, int nw1, int nw2) {
    
    int k = 0;
    int nw = nw1*nw2;
    srand(time(NULL));
    real* x = new real[n];
    for (int i = 0; i < n; i++) x[i] = 0;// rand() % 10;
    real* x_aux = new real[n]; 
    real* temp;

    ParallelFor pf(nw1);
    // std::vector<ParallelForReduce<real>> pfr(nw1);
    ParallelForReduce<real>* pfr[nw1];
    for (int i = 0; i < nw1; i++) {
        pfr[i] = new ParallelForReduce<real>(nw2);
    }

    ParallelForReduce<real> pfr1(nw2, true);   

    while (k < iterations) {

        if (nw1 > 0) {

            int size = n/nw1;

            pf.parallel_for(0, n, 1, size, [&](const long i) {
                real val = 0; 
                if (nw2 != 0) {
                    int idx = (i/size >= (nw1-1) ? nw1-1 : i/size);
                    pfr[idx]->parallel_reduce(val, 0, 0, n, 1, n/nw2, [&](const long j, real& partial) {
                        partial += (j != i ? A[i][j]*x[j] : 0.0);
                    },
                    [](real& total, const real partial) {
                        total += partial;
                    }, nw2);
                } else {
                    for (int j = 0; j < n; j++) {
                        val += (j != i ? A[i][j]*x[j] : 0);
                    }
                }
                x_aux[i] = (1/A[i][i])/(b[i] - val);
            }, nw1);
    
            temp = x_aux;
            x_aux = x;
            x = temp;
            k++;

        } else {
            real val;
            for (int i = 0; i < n; i++) {
                val = 0; 
                pfr1.parallel_reduce(val, 0, 0, n, 1, n/nw2, [&](const long j, real& partial) {
                    partial += (j != i ? A[i][j]*x[j] : 0.0);
                },
                [](real& total, const real partial) {
                    total += partial;
                }, nw2);
            }

            temp = x_aux;
            x_aux = x;
            x = temp;
            k++;
        }
    }

    delete x_aux;
    return x;

}

real* Jacobi::parallel_omp(int iterations, int nw1, int nw2) {

    real* x = new real[n];
    srand(time(NULL));
    for (int i = 0; i < n; i++) x[i] = 0;// rand() % 10;
    real* x_aux = new real[n]; 
    real val;
    real* temp;
    for (int k = 0; k < iterations; k++) {
        #pragma omp parallel for num_threads(nw1)
        for (int i = 0; i < n; i++) {
            val = 0;
            //#pragma omp parallel for reduction(+ : val) num_threads(nw1)
            for (int j = 0; j < n; j++) {
                val += (j != i ? A[i][j]*x[j] : 0);
            }
            x_aux[i] = (1/A[i][i])/(b[i] - val);
        }
        temp = x_aux;
        x_aux = x;
        x = temp;
    }
    delete x_aux;
    return x;

}