#include <audio/SoundListener.h>

#include <AL/al.h>

void SoundListener::updateALData()
{
    alListener3f(AL_POSITION, m_position.x, m_position.y, m_position.z);
    alListener3f(AL_VELOCITY, m_velocity.x, m_velocity.y, m_velocity.z);
}

SoundListener SoundListener::getInstance()
{
    SoundListener listener;
    return listener;
}