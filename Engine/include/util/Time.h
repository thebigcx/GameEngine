#pragma once

#include <chrono>

namespace Engine
{

typedef std::chrono::time_point<std::chrono::high_resolution_clock> TimePoint;

class Time
{
public:
    static double getDelta()
    {
        return m_delta;
    }

    static double getTime()
    {
        auto endTime = std::chrono::high_resolution_clock::now();

        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTime).time_since_epoch().count();
        auto stop = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();

        return (stop - start) * 0.001;
    }

private:
    static void update()
    {
        auto now = std::chrono::high_resolution_clock::now();

        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_lastFrame).time_since_epoch().count();
        auto stop = std::chrono::time_point_cast<std::chrono::microseconds>(now).time_since_epoch().count();

        m_delta = (stop - start) * 0.001;

        m_lastFrame = now;
    }

    static void init()
    {
        m_startTime = std::chrono::high_resolution_clock::now();
        m_lastFrame = m_startTime;
    }

    inline static double m_delta;
    inline static TimePoint m_startTime; // Beginning of program
    inline static TimePoint m_lastFrame; // Beginning of frame

    friend class Application;
};

}