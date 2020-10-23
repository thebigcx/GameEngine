#include <audio/SoundEngine.h>

#include <core/Logger.h>
#include <audio/SoundListener.h>

ALCdevice* SoundEngine::m_device;
ALCcontext* SoundEngine::m_context;

SoundSource SoundEngine::m_source2D;
SoundBuffer SoundEngine::m_buffer2D;

void SoundEngine::init()
{
    m_device = alcOpenDevice(nullptr);
    m_context = alcCreateContext(m_device, nullptr);
    alcMakeContextCurrent(m_context);

    m_source2D.create();

    SoundListener::getInstance().updateALData();
}

void SoundEngine::play2D(const SoundBuffer& buffer, bool loop)
{
    playFromSource(buffer, m_source2D, loop);
}

void SoundEngine::play2D(const std::string& path, bool loop)
{
    m_buffer2D.load(path);

    play2D(m_buffer2D, loop);
}

void SoundEngine::playFromSource(const SoundBuffer& buffer, const SoundSource& source, bool loop)
{
    if (source.isPlaying())
    {
        return;
    }

    alSourcei(source.m_id, AL_BUFFER, buffer.m_id);
    alSourcei(source.m_id, AL_LOOPING, (int)loop);
    alSourcePlay(source.m_id);
}

void SoundEngine::cleanUp()
{
    m_source2D.destroy();


    alcDestroyContext(m_context);
    alcCloseDevice(m_device);
}