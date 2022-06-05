#include <iostream>
#include <thread>
#include <vector>
#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace ff;


void print_matrix(int** A, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << A[i][j] << " ";
        }
        cout << "\n";
    }
} 

int main() {

    int nw1 = 4;
    int n = 9;
    int nw2 = 2;
    ParallelFor pf(nw1);
    // ParallelFor pf1(nw2);
    ParallelForReduce<int>* pfr[nw1];
    for (int i = 0; i < nw1; i++) {
        pfr[i] = new ParallelForReduce<int>(nw2);
    }

    int** A = new int*[n];
    int sums[n];
    for (int i = 0; i < n; i++) {
        A[i] = new int[n];
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = rand() % 10; 
        }
    }

    print_matrix(A, n);
    int size = n/nw1;

    for (int k = 0; k <= 10; k++) {
        pf.parallel_for(0, n, 1, size, [&](const long i) {
            int sum = 0; 
            int idx = (i/size >= (nw1-1) ? nw1-1 : i/size);
            // cout << idx << endl;
            // cout << this_thread::get_id() << endl;
            pfr[idx]->parallel_reduce(sum, 0, 0, n, 1, n/nw2, [&](const long j, int& partial) {
                partial += A[i][j];
            },
            [](int& total, const int partial) {
                total += partial;
            }, nw2);
            sums[i] = sum;
        }, nw1);

        for (int i = 0; i < n; i++) {
            cout << sums[i] << " ";
            sums[i] = 0; 
        }
        cout << endl;
    }


    // pf.parallel_for(0, n, 1, 0, [&](const long i) {
    //     std::cout << "Outer: " << i << "\n";
    //     int val = 0;
    //     pfr.parallel_reduce(val, 0, 0, n, 1, 0, [&](const long j, int& partial) {
    //             partial = A[i];
    //         },
    //         [](int& total, const int partial) {
    //             total += partial;
    //         }, nw2);)
    // }, nw1);

        // std:cout << "ZIOPERAZIOPERAZIOPERA\n";


}