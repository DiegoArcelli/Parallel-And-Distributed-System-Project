#include <iostream>
#include <ctime>
#include <cstdlib>

typedef double real;

void print_matrix(real** A, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cout << A[i][j] << " ";
        }
        std::cout << "\n";
    }
}

void print_array(real* a, int n) {
    for (int i = 0; i < n; i++) {
        std::cout << a[i] << " ";
    }
    std::cout << "\n";
}


int main() {

    int n = 3;
    int limit = 5000;

    real** A;
    real* b;
    real* x;
    A = new real*[n];
    b = new real[n];
    x = new real[n];
    for (int i = 0; i < n; i++) {
        A[i] = new real[n];
    }
    srand(time(NULL));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = rand() % 10;
        }
        if (A[i][i] == 0) {
            A[i][i] = (rand() % 9) + 1;
        }
        b[i] = rand() % 10;
        x[i] = rand();
    }

    print_matrix(A, n, n);
    std::cout << "\n";
    print_array(b, n);

    real val;
    real* x_new = new real[n]; 
    for (int k = 0; k < limit; k++) {
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

    std::cout << "\n\n";
    print_array(x, n);

    return 0;
}