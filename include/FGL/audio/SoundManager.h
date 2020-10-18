#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include <string>
#include <vector>

#include "SoundBuffer.h"
#include "Source.h"

class SoundManager
{
public:
    static void init();

    static void cleanUp();
    static void destroy();

    static void play2D(const SoundBuffer& buffer);
    static void play2D(const std::string& path);

    static void playFromSource(const SoundBuffer& buffer, const Source& source);

private:
    static ALCdevice* m_device;
    static ALCcontext* m_context;

    // For on-the-fly sound playback
    static Source m_source2D;
    static SoundBuffer m_buffer2D;
};