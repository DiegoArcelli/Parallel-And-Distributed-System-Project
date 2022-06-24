#include <cstdlib>

typedef long double real;

real** generate_diagonal_dominant_matrix(int n) {

    real** A = new real*[n];
    for (int i = 0; i < n; i++) {
        A[i] = new real[n];
        for (int j = 0; j < n; j++) {
            A[i][j] = rand() % 10;
        }
    }
}

bool is_diagonal_dominant(real** A, int n) {

    for (int i = 0; i < n; i++) {
        real sum = 0; 
        for (int j = 0; j < n; j++) {
            if (i != j) {
                sum += abs(A[i][j]);
            }
        }
        if (sum > abs(A[i][i])) {
            return false;
        }

    }

    return true;

}