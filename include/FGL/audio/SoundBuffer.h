#pragma once

#include <string>
#include <memory>

class SoundBuffer
{
public:
    SoundBuffer();
    ~SoundBuffer();

    // 16-bit bitdepth
    static SoundBuffer loadWAV(const std::string& path);

    void destroy();

    friend class Source;
    friend class SoundManager;

private:
    unsigned int m_id = 0;

    // SoundBuffer objects can only be created using a load* function
    SoundBuffer(unsigned int id);

};