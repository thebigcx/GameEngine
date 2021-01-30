#include <audio/AudioSource.h>
#include <audio/AudioBuffer.h>

namespace Engine
{

AudioSource::AudioSource(const Reference<AudioBuffer>& buffer)
    : m_buffer(buffer)
{
    alGenSources(1, &m_id);
    alSourcei(m_id, AL_BUFFER, m_buffer->m_id);
}

AudioSource::~AudioSource()
{
    alDeleteSources(1, &m_id);
}

Reference<AudioSource> AudioSource::create(const std::string& path)
{
    auto buffer = AudioBuffer::create(path);
    return Reference<AudioSource>(new AudioSource(buffer));
}

void AudioSource::play()
{
    alSourcePlay(m_id);
}

void AudioSource::pause()
{
    alSourcePause(m_id);
}

void AudioSource::resume()
{
    if (getState() == AudioSource::State::Paused)
    {
        play();
    }
}

void AudioSource::stop()
{
    alSourceStop(m_id);
}

void AudioSource::rewind()
{
    alSourceRewind(m_id);
}

void AudioSource::setLooped(bool looped)
{
    m_looped = looped;
    alSourcei(m_id, AL_LOOPING, static_cast<ALint>(looped));
}

void AudioSource::setGain(float gain)
{
    m_gain = gain;
    alSourcef(m_id, AL_GAIN, gain);
}

void AudioSource::setPitch(float pitch)
{
    m_pitch = pitch;
    alSourcef(m_id, AL_PITCH, pitch);
}

void AudioSource::setPosition(const math::vec3& position)
{
    m_position = position;
    alSource3f(m_id, AL_POSITION, position.x, position.y, position.z);
}

void AudioSource::setPosition(float x, float y, float z)
{
    m_position = { x, y, z };
    alSource3f(m_id, AL_POSITION, x, y, z);
}

void AudioSource::setVelocity(const math::vec3& velocity)
{
    m_velocity = velocity;
    alSource3f(m_id, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

void AudioSource::setVelocity(float x, float y, float z)
{
    m_velocity = { x, y, z };
    alSource3f(m_id, AL_VELOCITY, x, y, z);
}

AudioSource::State AudioSource::getState() const
{
    ALenum state;
    alGetSourcei(m_id, AL_SOURCE_STATE, &state);

    switch (state)
    {
        case AL_PLAYING:
            return AudioSource::State::Playing;
        case AL_PAUSED:
            return AudioSource::State::Paused;
        case AL_STOPPED:
            return AudioSource::State::Stopped;
        default:
            return AudioSource::State::Initial;
    };
}

}