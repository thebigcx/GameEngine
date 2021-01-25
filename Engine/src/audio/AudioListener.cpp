#include <audio/AudioListener.h>

#include <AL/al.h>

namespace Engine
{

AudioListener* AudioListener::getInstance()
{
    static AudioListener listener;
    return &listener;
}

void AudioListener::setPosition(float x, float y, float z)
{
    m_position = { x, y, z };
    alListener3f(AL_POSITION, m_position.x, m_position.y, m_position.z);
}

void AudioListener::setPosition(const math::vec3& position)
{
    m_position = position;
    alListener3f(AL_POSITION, m_position.x, m_position.y, m_position.z);
}

void AudioListener::setVelocity(float x, float y, float z)
{
    m_velocity = { x, y, z };
    alListener3f(AL_VELOCITY, m_velocity.x, m_velocity.y, m_velocity.z);
}

void AudioListener::setVelocity(const math::vec3& velocity)
{
    m_velocity = velocity;
    alListener3f(AL_VELOCITY, m_velocity.x, m_velocity.y, m_velocity.z);
}

void AudioListener::setGain(float gain)
{
    m_gain = gain;
    alListenerf(AL_GAIN, m_gain);
}

void AudioListener::setOrientation(const math::vec3& forward, const math::vec3& up)
{
    m_orientation[0] = forward;
    m_orientation[1] = up;

    alListenerfv(AL_ORIENTATION, &m_orientation[0].x);
}

void AudioListener::setOrientation(float fowardX, float forwardY, float forwardZ, float upX, float upY, float upZ)
{
    m_orientation[0] = { fowardX, forwardY, forwardZ };
    m_orientation[1] = { upX, upY, upZ };

    alListenerfv(AL_ORIENTATION, &m_orientation[0].x);
}

}