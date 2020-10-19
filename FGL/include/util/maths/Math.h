#pragma once

#include <cmath>
#include <ctime>
#include <cstdlib>

namespace Math 
{

struct Random
{
    static void initSeed()
    {
        std::srand(std::time(nullptr));
    }

    static double generate(int min, int max)
    {
        int rand = std::rand();
        return rand % max + min;
    }
};

static double map(double value, double istart, double istop, double ostart, double ostop)
{
    return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

        

static double clamp(double value, double min, double max)
{
    if (value < min)
    {
        return min;
    }

    if (value > max)
    {
        return max;
    }

    return value;
}

}