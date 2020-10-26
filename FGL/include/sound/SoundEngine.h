#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include <string>
#include <vector>

#include <sound/SoundBuffer.h>
#include <sound/SoundSource.h>

class SoundEngine
{
public:
    static void init();

    static void destroy();

    static void play(const SoundSource& source, bool loop);

private:
    static SoundEngine m_instance;

    static ALCdevice* m_device;
    static ALCcontext* m_context;
};