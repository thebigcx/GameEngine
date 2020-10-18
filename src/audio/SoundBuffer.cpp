#include <FGL/audio/SoundBuffer.h>

#include <AL/al.h>

#include <vendor/dr_libs/dr_wav.h>
#include <vendor/dr_libs/dr_mp3.h>
#include <FGL/core/Logger.h>

SoundBuffer::SoundBuffer()
{

}

SoundBuffer::SoundBuffer(unsigned int id)
    : m_id(id)
{
    
}

void SoundBuffer::load(FileType type, const std::string& path)
{
    if (m_id != 0)
    {
        destroy();
    }

    if (type == FileType::WAV)
    {
        m_id = loadWAV(path);
    }
    else if (type == FileType::MP3)
    {
        m_id = loadMP3(path);
    }
}

unsigned int SoundBuffer::loadWAV(const std::string& path)
{
    unsigned int buffer;
    alGenBuffers(1, &buffer);

    // Load the .wav file
    drwav wav;
    if (!drwav_init_file(&wav, path.c_str(), nullptr))
    {
        Logger::out("Could not open wave file: " + path);
        return 0;
    }

    size_t dataSize = wav.totalPCMFrameCount * wav.channels * sizeof(drwav_int16);
    drwav_int16* data = (drwav_int16*)malloc(dataSize);
    drwav_read_pcm_frames_s16(&wav, wav.totalPCMFrameCount, data);

    alBufferData(buffer, AL_FORMAT_STEREO16, data, dataSize, wav.sampleRate);

    drwav_uninit(&wav);
    
    return buffer;
}

unsigned int SoundBuffer::loadMP3(const std::string& path)
{
    unsigned int buffer;
    alGenBuffers(1, &buffer);

    // Load the .mp3 file
    drmp3 mp3;
    if (!drmp3_init_file(&mp3, path.c_str(), nullptr))
    {
        Logger::out("Could not open MP3 file: " + path);
        return 0;
    }

    drmp3_uint64 frameCount;
    drmp3_get_mp3_and_pcm_frame_count(&mp3, nullptr, &frameCount);

    size_t dataSize = frameCount * mp3.channels * sizeof(drmp3_int16);
    drmp3_int16* data = (drmp3_int16*)malloc(dataSize);
    drmp3_read_pcm_frames_s16(&mp3, frameCount, data);

    alBufferData(buffer, AL_FORMAT_STEREO16, data, dataSize, mp3.sampleRate);

    drmp3_uninit(&mp3);
    
    return buffer;
}

void SoundBuffer::destroy()
{
    if (m_id != 0)
    {
        alDeleteBuffers(1, &m_id);
    }
}

SoundBuffer::~SoundBuffer()
{
    // TODO: fix
    destroy();
}