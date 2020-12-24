#include <sound/SoundSource.h>

namespace Engine
{

SoundSource::SoundSource()
{
    
}

SoundSource::~SoundSource()
{
    destroy();
}

void SoundSource::loadSound(const std::string& path)
{
    m_buffer.load(path);

    alGenSources(1, &m_id);
    alSourcef(m_id, AL_GAIN, 1);
    alSourcef(m_id, AL_PITCH, 1);
    alSource3f(m_id, AL_POSITION, 0, 0, 0);
    alSourcei(m_id, AL_BUFFER, m_buffer.m_id);
}

std::shared_ptr<SoundSource> SoundSource::loadFile(const std::string& path)
{
    auto source = std::make_shared<SoundSource>();
    source->loadSound(path);
    return source;
}

void SoundSource::destroy()
{
    alDeleteSources(1, &m_id);
}

bool SoundSource::isPlaying() const
{
    ALenum state;
    alGetSourcei(m_id, AL_SOURCE_STATE, &state);

    return state == AL_PLAYING;
}

}