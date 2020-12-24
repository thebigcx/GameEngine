#pragma once

#include <string>
#include <memory>

namespace Engine
{

class SoundBuffer
{
public:
    SoundBuffer();
    ~SoundBuffer();

    void load(const std::string& path);

    // 16-bit bitdepth
    static unsigned int loadWAV(const std::string& path);
    static unsigned int loadMP3(const std::string& path);

    void destroy();

    friend class SoundSource;
    friend class SoundEngine;

private:
    unsigned int m_id = 0;

    // SoundBuffer objects can only be created using a load* function
    SoundBuffer(unsigned int id);

};

}