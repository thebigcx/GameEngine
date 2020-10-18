#pragma once

#include <string>
#include <memory>

class SoundBuffer
{
public:
    enum FileType
    {
        WAV,
        MP3
    };

    SoundBuffer();
    ~SoundBuffer();

    void load(FileType type, const std::string& path);

    // 16-bit bitdepth
    static unsigned int loadWAV(const std::string& path);
    static unsigned int loadMP3(const std::string& path);

    void destroy();

    friend class Source;
    friend class SoundManager;

private:
    unsigned int m_id = 0;

    // SoundBuffer objects can only be created using a load* function
    SoundBuffer(unsigned int id);

};