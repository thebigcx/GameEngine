#include <FGL/audio/Source.h>

Source::Source()
{
    
}

Source::~Source()
{
    destroy();
}

void Source::create()
{
    alGenSources(1, &m_id);
    alSourcef(m_id, AL_GAIN, 1);
    alSourcef(m_id, AL_PITCH, 1);
    alSource3f(m_id, AL_POSITION, 0, 0, 0);
}

void Source::destroy()
{
    alDeleteSources(1, &m_id);
}

bool Source::isPlaying() const
{
    ALenum state;
    alGetSourcei(m_id, AL_SOURCE_STATE, &state);

    return state == AL_PLAYING;
}