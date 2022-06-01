#include <chrono>
#include <string>

using time_point = std::chrono::system_clock::time_point;

#ifndef TIMER_H
#define TIMER_H

class timer {
private:
    time_point start;
    time_point stop;
    int64_t duration;
    std::string text;
    
public:
    timer(std::string text);
    ~timer();
};

#endif

