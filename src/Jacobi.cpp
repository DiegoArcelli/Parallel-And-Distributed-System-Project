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
    real* temp;
    
    {
        timer t("seq");
        real val;
        for (int k = 0; k < iterations; k++) {
            for (int i = 0; i < n; i++) {
                val = 0;
                for (int j = 0; j < n; j++) {
                    val += (j != i ? A[i][j]*x[j] : 0);
                }
                x_aux[i] = (b[i] - val)/A[i][i];
            }
            temp = x_aux;
            x_aux = x;
            x = temp;
            // for (int i = 0; i < n; i++)
            //     x[i] = x_aux[i];
        }
    }

    delete x_aux;
    return x;
}

real* Jacobi::parallel(std::string mode, int iterations, int nw) {
    
    if (mode == "threads") {
        return this->parallel_threads(iterations, nw);
    } else if (mode == "ff") { 
        return this->parallel_ff(iterations, nw);
    } else if (mode == "omp") {
        return this->parallel_omp(iterations, nw);
    }

    return nullptr;
}

real* Jacobi::parallel_threads(int iterations, int nw) {

    srand(time(NULL));
    real* x = new real[n];
    for (int i = 0; i < n; i++) x[i] = 0;// rand() % 10;
    real* x_aux= new real[n]; 
    real* temp;

    {
        timer t("par");
        int k = 0;
        std::barrier copy_barrier(nw, []() { return; });
        std::barrier next_iteration_barrier(nw, []() { return; });
        std::thread tids[nw];

        std::function<void(int, int)> thread_map_task = [&](int start, int end){

            while (k < iterations) {

                int i, j;
                real val;
                // do the first map calling the reduce operations
                for (i = start; i <= end; i++) {
                    val = 0;
                    for (j = 0; j < n; j++) {
                        val += (j != i ? A[i][j]*x[j] : 0);
                    }
                    x_aux[i] = (b[i] - val)/A[i][i];
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
            int start = i*size;
            int end = (i != nw-1 ? start + size - 1 : n-1);
            tids[i] = std::thread(thread_map_task, start, end);
        }

        for (int i = 0; i < nw; i++) {
            tids[i].join();
        }
    }

    delete x_aux;

    return x;

}

real* Jacobi::parallel_ff(int iterations, int nw) {
    
    srand(time(NULL));
    real* x = new real[n];
    for (int i = 0; i < n; i++) x[i] = 0;// rand() % 10;
    real* x_aux = new real[n]; 
    real* temp;

    {
        timer t("fff");
        ParallelFor pf(nw);
        int k = 0; 
        int size = n/nw;
        // real val;
        int j;

        while (k < iterations) {
            pf.parallel_for(0, n, 1, size, [&](const long i) {
                real val = 0;
                int j; 
                for (j = 0; j < n; j++) {
                    val += (j != i ? A[i][j]*x[j] : 0);
                }
                x_aux[i] = (b[i] - val)/A[i][i];
            }, nw);

            temp = x_aux;
            x_aux = x;
            x = temp;
            k++;
        }
    }

    delete x_aux;
    return x;

}

real* Jacobi::parallel_omp(int iterations, int nw) {

    real* x = new real[n];
    srand(time(NULL));
    for (int i = 0; i < n; i++) x[i] = 0;// rand() % 10;
    real* x_aux = new real[n]; 
    real* temp;
    int i, j, val;

    {
        timer t("omp");
        real val;
        for (int k = 0; k < iterations; k++) {
            #pragma omp parallel for private(i, j, val) num_threads(nw)
            for (i = 0; i < n; i++) {
                val = 0;
                for (j = 0; j < n; j++) {
                    val += (j != i ? A[i][j]*x[j] : 0);
                }
                x_aux[i] = (b[i] - val)/A[i][i];
            }
            temp = x_aux;
            x_aux = x;
            x = temp;
        }
    }
    delete x_aux;
    return x;

}