#include <cstdlib>
#include <ctime>


inline int sign(double x) {
    return ((x >= 0) - (x < 0));
}

inline double* generate_random_vector(int n) {
    
    srand(time(NULL));
    double* b = new double[n];

    for (int i = 0; i < n; i++) {
        b[i] = (rand() % 20) - 10;
    }
    return b;
} 

inline double** generate_diagonal_dominant_matrix(int n) {

    srand(time(NULL));
    double** A = new double*[n];
    
    for (int i = 0; i < n; i++) {
        A[i] = new double[n];
        for (int j = 0; j < n; j++) {
            A[i][j] = (rand() % 20 ) - 10;
        }
    }
 
    for (int i = 0; i < n; i++) {
        double sum = 0; 
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
