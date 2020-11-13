#pragma once

#include <vector>

#include <core/Core.h>
#include <util/Time.h>
#include <math/Rect.h>
#include <renderer/Texture2D.h>

class Animation
{
public:
    Animation() {};

    static Shared<Animation> create(Shared<Texture2D> texture);

    void pushFrame(const FloatRect& frame);
    void setFrames(const std::initializer_list<FloatRect> frames);

    void setFrameInterval(float interval)
    {
        m_interval = interval;
    }

    float getFrameInterval() const { return m_interval; }

    const FloatRect& getCurrentFrame() const;

    inline void update()
    {
        if (Time::getTime() - m_lastTime > m_interval)
        {
            m_lastTime = Time::getTime();

            m_currentFrameIndex++;
            if (m_currentFrameIndex >= m_frames.size())
            {
                m_currentFrameIndex = 0;
            }
        }
    }

private:
    Shared<Texture2D> m_pTexture;
    std::vector<FloatRect> m_frames;

    int m_currentFrameIndex = 0;

    float m_interval = 0;

    double m_lastTime = 0;
};