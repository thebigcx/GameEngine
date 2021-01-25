#pragma once

#include <string>
#include <memory>

#include <core/Core.h>

namespace Engine
{

class AudioBuffer
{
    friend class AudioSource;
    friend class SoundEngine;

private:
    AudioBuffer(const std::string& filepath);
    AudioBuffer(uint32_t id);

    static Shared<AudioBuffer> create(const std::string& path);

    // 16-bit bitdepth
    static void loadWAV(const std::string& path, uint32_t buffer);
    static void loadMP3(const std::string& path, uint32_t buffer);

    uint32_t m_id = 0;

public:
    ~AudioBuffer();
};

}