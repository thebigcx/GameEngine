#pragma once

#include <maths/math.h>

namespace Engine
{

class AudioListener
{
private:
    AudioListener();
    ~AudioListener();

public:
    inline const math::vec3& getPosition() const { return m_position; }
    inline const math::vec3& getVelocity() const { return m_velocity; }
    inline const math::vec3& getForward() const { return m_orientation[0]; }
    inline const math::vec3& getUp() const { return m_orientation[1]; }

    /**
     * Orientation is represented as 3 floats for forward, and 3 floats for up
     * @returns First float in orientation array.
     */
    inline const float* getOrientation() const { return &m_orientation[0].x; }

    void setPosition(const math::vec3& position);
    void setPosition(float x, float y, float z);

    void setVelocity(const math::vec3& velocity);
    void setVelocity(float x, float y, float z);

    // Gain of the listener
    void setGain(float gain);
    inline float getGain() const { return m_gain; }

    void setOrientation(const math::vec3& forward, const math::vec3& up);
    void setOrientation(float fowardX, float forwardY, float forwardZ, float upX, float upY, float upZ);

    // There can only be one listener per scene - singleton.
    static AudioListener* getInstance();

private:
    float m_gain;

    math::vec3 m_position;
    math::vec3 m_velocity;

    // Orientation
    math::vec3 m_orientation[2];

};

}