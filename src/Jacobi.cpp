#include "Jacobi.hpp"
#include <ctime>
#include <vector>
#include <functional>
#include <cstdlib>
#include <thread>
#include <iostream>
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
    for (int i = 0; i < n; i++) x[i] = 10;// rand() % 10;
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
    std::barrier next_iteration_barrier(nw1*nw2, []() { return; });
    std::thread tids[nw];
    std::vector<std::pair<int, int>> reduce_chunks;
    std::vector<std::pair<int, int>> map_chunks;
    std::vector<std::vector<real>> partials;

    // std::barrier<std::function<void(void)>> reduce_barriers[nw1];
    
    // std::vector<std::barrier<>> reduce_barriers(nw1);
    for (int i = 0; i < nw1; i++) {
        partials.push_back(std::vector<real>(nw2));
        // reduce_barriers[i] = std::barrier(nw2);
    }

    int reduce_chunk_size = n/nw2;
    for (int i = 0; i < nw2; i++) {
        int start = i*reduce_chunk_size;
        int end = (i == nw-1 ? n-1 : start + reduce_chunk_size - 1);
        reduce_chunks.push_back(std::pair<int, int>(start, end));
    }

    int map_chunk_size = n/nw1;
    for (int i = 0; i < nw1; i++) {
        int start = i*map_chunk_size;
        int end = (i == nw1-1 ? n-1 : start + map_chunk_size - 1);
        map_chunks.push_back(std::pair<int, int>(start, end));
    }


    auto thread_map_task = [&](int start, int end, int reduce_start, int reduce_end){

        while (k < iterations) {

            // do the first map calling the reduce operations
            for (int i = start; i <= end; i++) {
    
                
                
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

    auto thread_reduce_task = [&](int start, int end, int row, int p_i, int p_j) {
        while (k < iterations) {
            partials[p_i][p_j] = 0.0;
            for (int i = start; i <= end; i++){
                partials[p_i][p_j] += (i != row ? A[row][i]*x[i] : 0.0);
            }
            next_iteration_barrier.arrive_and_wait();
        }
    };


    for (int i = 0; i < nw; i++) {
        // thread that have to handle the map
        if (i % nw == 0) {

        // thread that only have to reduce
        } else {

        }
    }


    for (int i = 0; i < nw; i++) {
        tids[i].join();
    }

    return nullptr;

}

real* Jacobi::parallel_ff(int iterations, int nw1, int nw2) {
    
    int k = 0;
    int nw = nw1*nw2;
    srand(time(NULL));
    real* x = new real[n];
    for (int i = 0; i < n; i++) x[i] = 10;// rand() % 10;
    real* x_aux = new real[n]; 
    real* temp;

    ParallelFor pf(nw1);
    // std::vector<ParallelForReduce<real>> pfr(nw1);
    ParallelForReduce<real>* pfr[nw1];
    for (int i = 0; i < nw1; i++) {
        pfr[i] = new ParallelForReduce<real>(nw2);
    }    

    while (k < iterations) {
        pf.parallel_for(0, n, 1, 0, [&](const long i) {
            real val = 0; 
            int idx = (i/nw1 >= (nw1-1) ? nw1-1 : i/nw1);
            pfr[idx]->parallel_reduce(val, 0, 0, n, 1, 0, [&](const long j, real& partial) {
                partial = (j != i ? A[i][j]*x[j] : 0.0);
            },
            [](real& total, const real partial) {
                total += partial;
            }, nw2);
            x_aux[i] = (1/A[i][i])/(b[i] - val);
            DEBUG(i);
        }, nw1);
        std::cout << "ENDED PARALLEL FOR\n";
   
        temp = x_aux;
        x_aux = x;
        x = temp;
        k++;
    }

    return x;

}

real* Jacobi::parallel_omp(int iterations, int nw1, int nw2) {

    int k = 0;
    srand(time(NULL));
    real* x = new real[n];
    for (int i = 0; i < n; i++) x[i] = 0;// rand() % 10;
    real* x_new = new real[n]; 

    return nullptr;

}