#pragma once

#include <cmath>
#include <functional>
#include <stack>
#include <chrono>

struct TASK {
    std::function<double(double)> func = [](double x) -> double {
        return x * std::cos(1 / x);
    };

    double a = 1.1;
    double b = 10000.1;
};

struct SUBTASK_RECORD {
    double a;
    double b;
    double sum;
};

class Timer
{
private:
    using clock_t = std::chrono::high_resolution_clock;
    using second_t = std::chrono::duration<double, std::ratio<1>>;

    std::chrono::time_point<clock_t> m_beg;

public:
    Timer() : m_beg(clock_t::now())
    {
    }

    void reset()
    {
        m_beg = clock_t::now();
    }

    double elapsed() const
    {
        return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
    }
};
