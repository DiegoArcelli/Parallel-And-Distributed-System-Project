#include <iostream>
#include "Jacobi.hpp"

int main(int argc, char *argv[]) {
    
    srand(time(NULL));

    if (argc < 5) {
        std::cout << "Usage: ./main n nw iters mode\
                    \nOptions for mode:\n0) Execute all \n1) Execute sequential version only\
                    \n2) Execute parallel versions only\n3) Execute only C++ threads version\
                    \n4) Execute FastFlow version only\n5) Execute Open MP version only\n";
        return 1; 
    }

    int n = atoi(argv[1]);
    int nw = atoi(argv[2]);
    int iters = atoi(argv[3]);


    /*
        mode == 0: execute all
        mode == 1: execute only sequential
        mode == 2: execute only parallels
        mode == 3: execute only C++ threads
        mode == 4: execute only fastflow
        mode == 5: execute only omp

    */
    int mode = atoi(argv[4]);

    Jacobi prob(n);

    real* x;

    if (mode < 2) {
        x = prob.sequential(iters);
    }

    if (mode == 0 || mode == 2 || mode == 3) {
        x = prob.parallel("threads", iters, nw);
    }

    if (mode == 0 || mode == 2 || mode == 4) {
        x = prob.parallel("ff", iters, nw);
    }


    if (mode == 0 || mode == 2 || mode == 5) {
        x = prob.parallel("omp", iters, nw);
    }




}