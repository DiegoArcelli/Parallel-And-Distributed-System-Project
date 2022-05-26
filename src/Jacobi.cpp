#include "Jacobi.hpp"
#include <ctime>
#include <functional>
#include <cstdlib>
#include <thread>
#include <iostream>
#include <ff/parallel_for.hpp>
#include <barrier>
#include <omp.h>

using namespace ff;

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
    real* x_new = new real[n]; 
    real val;
    for (int k = 0; k < iterations; k++) {
        for (int i = 0; i < n; i++) {
            val = 0;
            for (int j = 0; j < n; j++) {
                val += (j != i ? A[i][j]*x[j] : 0);
            }
            x_new[i] = (1/A[i][i])/(b[i] - val);
        }
        for (int i = 0; i < n; i++)
            x[i] = x_new[i];
    }
    delete x_new;
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

    int k = 0;
    srand(time(NULL));
    real* x = new real[n];
    for (int i = 0; i < n; i++) x[i] = 0;// rand() % 10;
    real* x_new = new real[n]; 

    std::barrier copy_barrier(nw, []() {return; });
    std::barrier next_iteration_barrier(nw, []() {return; });

    std::function<void(int, int)> thread_task = [&](int start, int end) {
        real val;
        while (k < iterations) {
            for (int i = start; i <= end; i++) {
                val = 0;
                for (int j = 0; j < n; j++) {
                    val += (j != i ? A[i][j]*x[j] : 0);
                }
                x_new[i] = (1/A[i][i])/(b[i] - val);
            }

            copy_barrier.arrive_and_wait();

            for (int i = start; i <= end; i++) {
                x[i] = x_new[i];
            }

            if (start == 0) {
                k+=1;
            }

            copy_barrier.arrive_and_wait();
        }
        
    };

    std::thread tids[nw];
    int chunk_size = n/nw;
    for (int i = 0; i < nw; i++) {
        int start = i*chunk_size;
        int end = (i == nw - 1 ? n-1 : start + chunk_size - 1);
        // std::cout << "Thread " << i << ": (" << start << ", " << end << ")\n";
        tids[i] = std::thread(thread_task, start, end);
    }
    for (int i = 0; i < nw; i++) {
        tids[i].join(); 
    }


    return x;

}

real* Jacobi::parallel_ff(int iterations, int nw) {
    
    int k = 0;
    srand(time(NULL));
    real* x = new real[n];
    for (int i = 0; i < n; i++) x[i] = 0;// rand() % 10;
    real* x_new = new real[n]; 

    ParallelFor pf(nw);
    std::vector<std::pair<int, int>> chunks;

    int chunk_size = n/nw;
    for (int i = 0; i < nw; i++) {
        int start = i*chunk_size;
        int end = (i == nw - 1 ? n-1 : start + chunk_size - 1);
        chunks.push_back(std::pair(start, end));
    }

    while (k < iterations) {

        for (std::vector<std::pair<int,int>>::iterator itr = chunks.begin(); itr != chunks.end(); itr++) {
            pf.parallel_for(itr->first, itr->second, 1, 0, [&](const long i){
                
            }, nw);
        }

        for (std::vector<std::pair<int,int>>::iterator itr = chunks.begin(); itr != chunks.end(); itr++) {
            pf.parallel_for(itr->first, itr->second, 1, 0, [&](const long i){
                
            }, nw);
        }


    }

    // for (auto itr = chunks.begin(); itr != chunks.end(); itr++) {

    // }


    return x;

}

real* Jacobi::parallel_omp(int iterations, int limit) {

    int k = 0;
    srand(time(NULL));
    real* x = new real[n];
    for (int i = 0; i < n; i++) x[i] = 0;// rand() % 10;
    real* x_new = new real[n]; 

    #pragma omp parallel num_threads(nw)
    {    

        #pragma omp barrier


        #pragma omp barrier


    }


}