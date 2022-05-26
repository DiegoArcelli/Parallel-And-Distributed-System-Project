#include "utils.hpp"
#include <string>

class Jacobi {
private:
    int n;
    real** A;
    real* b;
    real* parallel_threads(int iterations, int nw);
    real* parallel_ff(int iterations, int nw);
    real* parallel_omp(int iterations, int nw);
public:
    Jacobi(int n);
    Jacobi(int n, real** A, real *b) : n(n), A(A), b(b) {}
    real* sequential(int iterations);
    real* parallel(std::string mode, int iterations, int nw);
};