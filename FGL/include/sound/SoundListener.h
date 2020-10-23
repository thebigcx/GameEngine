#pragma once

#include <core/Console.h>
#include <util/math/vector/Vector3.h>

class SoundListener
{
public:
    SoundListener() {};

    void updateALData();

    inline Vector3f getPosition() const
    {
        return m_position;
    }

    inline Vector3f getVelocity() const
    {
        return m_velocity;
    }

    inline void setPosition(const Vector3f position)
    {
        m_position = position;
    }

    inline void setVelocity(const Vector3f velocity)
    {
        m_velocity = velocity;
    }

    static SoundListener getInstance();

private:
    Vector3f m_position;
    Vector3f m_velocity;

};