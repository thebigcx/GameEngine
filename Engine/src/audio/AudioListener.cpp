#include <audio/AudioListener.h>
#include <scene/GameObject.h>
#include <util/Transform.h>

#include <AL/al.h>

namespace Engine
{
AudioListener::AudioListener()
{

}

AudioListener::~AudioListener()
{

}

/*
AudioListener* AudioListener::getInstance()
{
    static AudioListener listener;
    return &listener;
}

void AudioListener::setPosition(float x, float y, float z)
{
    m_position = { x, y, z };

    if (isPrimary())
        alListener3f(AL_POSITION, m_position.x, m_position.y, m_position.z);
}

void AudioListener::setPosition(const math::vec3& position)
{
    m_position = position;

    if (isPrimary())
        alListener3f(AL_POSITION, m_position.x, m_position.y, m_position.z);
}

void AudioListener::setVelocity(float x, float y, float z)
{
    m_velocity = { x, y, z };

    if (isPrimary())
        alListener3f(AL_VELOCITY, m_velocity.x, m_velocity.y, m_velocity.z);
}

void AudioListener::setVelocity(const math::vec3& velocity)
{
    m_velocity = velocity;

    if (isPrimary())
        alListener3f(AL_VELOCITY, m_velocity.x, m_velocity.y, m_velocity.z);
}
*/
void AudioListener::setGain(float gain)
{
    m_gain = gain;

    if (isPrimary())
        alListenerf(AL_GAIN, m_gain);
}
/*
void AudioListener::setOrientation(const math::vec3& forward, const math::vec3& up)
{
    m_orientation[0] = forward;
    m_orientation[1] = up;

    if (isPrimary())
        alListenerfv(AL_ORIENTATION, &m_orientation[0].x);
}

void AudioListener::setOrientation(float fowardX, float forwardY, float forwardZ, float upX, float upY, float upZ)
{
    m_orientation[0] = { fowardX, forwardY, forwardZ };
    m_orientation[1] = { upX, upY, upZ };

    if (isPrimary())
        alListenerfv(AL_ORIENTATION, &m_orientation[0].x);
}
*/
void AudioListener::onTransformChange(const Transform& transform)
{
    if (!isPrimary())
        return;
        
    math::vec3 orientation[2];
    math::vec3 position;
    math::vec3 velocity;

    position = transform.getTranslation();
    orientation[0] = math::quat(transform.getRotation()) * math::vec3(0, 0, 1);
    orientation[1] = math::quat(transform.getRotation()) * math::vec3(0, 1, 0);

    alListenerfv(AL_ORIENTATION, &orientation[0].x);
    alListenerfv(AL_VELOCITY, &velocity.x);
    alListenerfv(AL_POSITION, &position.x);
}

void AudioListener::setPrimary(bool primary)
{
    m_primary = primary;
}

}