#include <cstdlib>
#include <ctime>

typedef float real;

inline int sign(real x) {
    return ((x >= 0) - (x < 0));
}

inline real* generate_random_vector(int n) {
    
    srand(time(NULL));
    real* b = new real[n];

    for (int i = 0; i < n; i++) {
        b[i] = (rand() % 20) - 10;
    }
    return b;
} 

inline real** generate_diagonal_dominant_matrix(int n) {

    srand(time(NULL));
    real** A = new real*[n];
    
    for (int i = 0; i < n; i++) {
        A[i] = new real[n];
        for (int j = 0; j < n; j++) {
            A[i][j] = (rand() % 20 ) - 10;
        }
    }
 
    for (int i = 0; i < n; i++) {
        real sum = 0; 
        for (int j = 0; j < n; j++) {
            if (i != j) {
                sum += abs(A[i][j]);
            }
        }
        if (sum >= abs(A[i][i])) {
            A[i][i] = A[i][i] + sign(A[i][i])*(sum - abs(A[i][i])) + sign(A[i][i])*1 ;
        }
    }

    return A;
}
