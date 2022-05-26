#include <iostream>
#include "Jacobi.hpp"
#include "utils.hpp"

int main(int argc, char *argv[]) {
    
    srand(time(NULL));
    int n = 18;
    
    real** A = new real*[n];
    real* b = new real[n];
    for (int i = 0; i < n; i++) {
        A[i] = new real[n];
        for (int j = 0; j < n; j++) {
            A[i][j] = rand() % 10;
        }
        if (A[i][i] == 0) A[i][i] = (rand() % 9) + 1;
        b[i] = rand() % 10;
    }

    Jacobi prob(n, A, b);

    real* x;

    x = prob.sequential(1000);

    for (int i = 0; i < n; i++)
        std::cout << x[i] << " ";
    std::cout << "\n";


    x = prob.parallel(1000, 4);
    for (int i = 0; i < n; i++)
        std::cout << x[i] << " ";
    std::cout << "\n";



}