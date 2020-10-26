#include <sound/SoundListener.h>

#include <AL/al.h>

SoundListener SoundListener::getInstance()
{
    SoundListener listener;
    return listener;
}

void SoundListener::setPosition(float x, float y, float z)
{
    m_position = { x, y, z };
    alListener3f(AL_POSITION, m_position.x, m_position.y, m_position.z);
}

void SoundListener::setVelocity(float x, float y, float z)
{
    m_velocity = { x, y, z };
    alListener3f(AL_VELOCITY, m_velocity.x, m_velocity.y, m_velocity.z);
}