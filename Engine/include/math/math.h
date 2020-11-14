#pragma once

#include <cmath>
#include <ctime>
#include <cstdlib>

namespace math
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

static constexpr double PI = 3.14159265358979323846;

static double asRadians(int deg)
{
    return deg * 0.01745329251994329576923690768489;
}

static int asDegrees(double rad)
{
    return rad * (180 / PI);
}

} // namespace Math

