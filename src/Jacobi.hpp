#include "utils.hpp"
#include <string>

class Jacobi {
private:
    int n;
    double** A;
    double* b;
    double* parallel_threads(int iterations, int nw);
    double* parallel_ff(int iterations, int nw);
    double* parallel_omp(int iterations, int nw);
public:
    Jacobi(int n);
    Jacobi(int n, double** A, double *b) : n(n), A(A), b(b) {}
    double* sequential(int iterations);
    double* parallel(std::string mode, int iterations, int nw);
};