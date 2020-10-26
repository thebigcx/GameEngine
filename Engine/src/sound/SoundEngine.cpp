#include <sound/SoundEngine.h>

#include <core/Console.h>
#include <sound/SoundListener.h>

ALCdevice* SoundEngine::m_device;
ALCcontext* SoundEngine::m_context;

void SoundEngine::init()
{
    m_device = alcOpenDevice(nullptr);
    m_context = alcCreateContext(m_device, nullptr);
    alcMakeContextCurrent(m_context);
}

void SoundEngine::play(const SoundSource& source, bool loop)
{
    if (source.isPlaying())
    {
        return;
    }

    alSourcei(source.m_id, AL_LOOPING, (int)loop);
    alSourcePlay(source.m_id);
}

void SoundEngine::destroy()
{
    alcDestroyContext(m_context);
    alcCloseDevice(m_device);
}