#pragma once

#include <chrono>

template<typename Clock>
class Stopwatch
{
public:
    using time_point = typename Clock::time_point;

    Stopwatch() :
        m_origin(Clock::now())
    {}

    void Start()
    {
        m_origin = Clock::now();
    }

    //'duration' example: std::chrono::milliseconds
    template<typename duration>
    long long GetElapsedTime()
    {
        const time_point current = Clock::now();
        return std::chrono::duration_cast<duration>(current - m_origin).count();
    }

private:
    time_point m_origin;
};