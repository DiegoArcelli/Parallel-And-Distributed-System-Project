#include <iostream>
#include "Jacobi.hpp"
#include "timer.hpp"

int main(int argc, char *argv[]) {
    
    srand(time(NULL));

    if (argc < 4) {
        std::cout << "Usage: ./main n nw iters\n";
        return 1; 
    }

    int n = atoi(argv[1]);
    int nw = atoi(argv[2]);
    int iters = atoi(argv[3]);
    
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

    {
        timer t("seq");
        x = prob.sequential(iters);
    }

    // for (int i = 0; i < n; i++)
    //     std::cout << x[i] << " ";
    // std::cout << "\n";

    {
        timer t("fff");
        x = prob.parallel("ff", iters, nw);
    }
    // for (int i = 0; i < n; i++)
    //     std::cout << x[i] << " ";
    // std::cout << "\n";


    {
        timer t("par");
        x = prob.parallel("threads", iters, nw);
    }
    // for (int i = 0; i < n; i++)
    //     std::cout << x[i] << " ";
    // std::cout << "\n";


    {
        timer t("omp");
        x = prob.parallel("omp", iters, nw);
    }
    // for (int i = 0; i < n; i++) {
    //     std::cout << x[i] << " ";
    // }
    // std::cout << "\n";




}