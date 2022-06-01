#include <iostream>
#include <vector>
#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>

using namespace std;
using namespace ff;

int main() {

    int nw = 4;
    int n = 128;
    ParallelFor pf(nw);

    pf.parallel_for(0, n, 1, 0, [](const long i) {
        std::cout << i << "\n";
    }, nw); 

    std:cout << "PROVAPROVAPROVA\n";


}