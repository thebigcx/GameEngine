#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include <string>
#include <vector>

#include <core/Core.h>

namespace Engine
{

class AudioController
{
public:
    AudioController();
    ~AudioController();

    void initialize();
    void finalize();

    static AudioController* getInstance();

private:
    ALCdevice* m_device;
    ALCcontext* m_context;
};

}