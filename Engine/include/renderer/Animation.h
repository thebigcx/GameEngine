#pragma once

#include <vector>

#include <core/Core.h>
#include <util/Time.h>
#include <maths/rect/rect.h>
#include <renderer/Texture2D.h>

namespace Engine
{

class Animation
{
public:
    Animation() = default;

    static Reference<Animation> create(Reference<Texture2D> texture);

    void pushFrame(const math::frect& frame);
    void setFrames(const std::initializer_list<math::frect> frames);

    void setFrameInterval(float interval)
    {
        m_interval = interval;
    }

    float getFrameInterval() const { return m_interval; }

    const math::frect& getCurrentFrame() const;

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
    Reference<Texture2D> m_pTexture;
    std::vector<math::frect> m_frames;

    unsigned int m_currentFrameIndex = 0;

    float m_interval = 0;

    double m_lastTime = 0;
};

}