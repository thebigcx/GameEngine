#pragma once

class SoundListener
{
private:
    struct vec3
    {
        float x, y, z;
    };

public:
    SoundListener() {};

    inline vec3 getPosition() const { return m_position; }
    inline vec3 getVelocity() const { return m_velocity; }

    void setPosition(float x, float y, float z);
    void setVelocity(float x, float y, float z);

    static SoundListener getInstance();

private:
    vec3 m_position;
    vec3 m_velocity;

};