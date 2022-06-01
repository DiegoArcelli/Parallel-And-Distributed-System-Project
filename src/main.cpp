#include <iostream>
#include "Jacobi.hpp"
#include "utils.hpp"
#include "timer.hpp"

int main(int argc, char *argv[]) {
    
    srand(time(NULL));
    int n = 18;
    
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
        x = prob.sequential(1);
    }

    for (int i = 0; i < n; i++)
        std::cout << x[i] << " ";
    std::cout << "\n";

    {
        timer t("fff");
        x = prob.parallel("ff", 1, 4, 2);
    }
    for (int i = 0; i < n; i++)
        std::cout << x[i] << " ";
    std::cout << "\n";


    // x = prob.parallel("threads", 1000, 4, 2);
    // for (int i = 0; i < n; i++)
    //     std::cout << x[i] << " ";
    // std::cout << "\n";



}