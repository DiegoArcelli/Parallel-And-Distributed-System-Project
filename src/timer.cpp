#include "timer.hpp"
#include <iostream>

timer::timer(std::string text) {
    this->start = std::chrono::system_clock::now();
    this->text = text;
}

timer::~timer() {
    this->stop = std::chrono::system_clock::now();
    this->duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
    std::cout << text << ": " << duration << " nanoseconds\n";
}   