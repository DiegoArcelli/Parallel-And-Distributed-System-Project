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
    
    real** A = new real*[n];
    real* b = new real[n];
    for (int i = 0; i < n; i++) {
        A[i] = new real[n];
        for (int j = 0; j < n; j++) {
            A[i][j] = 10;//rand() % 10;
        }
        if (A[i][i] == 0) A[i][i] = (rand() % 9) + 1;
        b[i] = 10;//rand() % 10;
    }

    Jacobi prob(n, A, b);

    real* x;

    if (mode < 2) {
        // timer t("seq");
        x = prob.sequential(iters);
    }

    // for (int i = 0; i < n; i++)
    //     std::cout << x[i] << " ";
    // std::cout << "\n";

    // for (int i = 0; i < n; i++)
    //     std::cout << x[i] << " ";
    // std::cout << "\n";


    if (mode == 0 || mode == 2 || mode == 3) {
        // timer t("par");
        x = prob.parallel("threads", iters, nw);
    }
    // for (int i = 0; i < n; i++)
    //     std::cout << x[i] << " ";
    // std::cout << "\n";
    if (mode == 0 || mode == 2 || mode == 4) {
        // timer t("fff");
        x = prob.parallel("ff", iters, nw);
    }


    if (mode == 0 || mode == 2 || mode == 5) {
        // timer t("omp");
        x = prob.parallel("omp", iters, nw);
    }
    // for (int i = 0; i < n; i++) {
    //     std::cout << x[i] << " ";
    // }
    // std::cout << "\n";




}