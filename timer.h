#pragma once

#include <chrono>

namespace Qoolkie
{

class Timer
{
    private:
        typedef std::chrono::high_resolution_clock clock;
        typedef std::chrono::duration<double, std::ratio<1> > second;

        std::chrono::time_point<clock> beg;

    public:
        Timer() : beg(clock::now()) { }
        Timer(Timer &&oth) : beg(std::move(oth.beg)) { }

        Timer(const Timer& oth) = delete;
        Timer& operator=(const Timer& oth) = delete;
        ~Timer() = default;

        void reset()
        {
            beg = clock::now();
        }

        double elapsed() const
        {
            return std::chrono::duration_cast<second>(clock::now() - beg).count();
        }
};

}
