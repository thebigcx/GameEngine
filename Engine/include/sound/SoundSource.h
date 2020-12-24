#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include "SoundBuffer.h"

namespace Engine
{

class SoundSource
{
public:
    SoundSource();
    ~SoundSource();

    void loadSound(const std::string& path);

    static std::shared_ptr<SoundSource> loadFile(const std::string& path);

    void setPosition(float x, float y, float z);
    void setGain(float gain);
    void setPitch(float pitch);    

    void destroy();

    bool isPlaying() const;

    friend class SoundEngine;

private:
    struct vec3
    {
        float x, y, z;
    };

    unsigned int m_id;

    SoundBuffer m_buffer;

    vec3 m_position;
};

}