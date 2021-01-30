#include <audio/AudioBuffer.h>

#include <AL/al.h>

#include <dr_libs/dr_wav.h>
#include <dr_libs/dr_mp3.h>
#include <core/Logger.h>

namespace Engine
{

AudioBuffer::AudioBuffer(const std::string& filepath)
{
    alGenBuffers(1, &m_id);

    auto extension = filepath.substr(filepath.find_last_of(".") + 1);
    if (extension == "wav")
    {
        loadWAV(filepath, m_id);
    }
    else if (extension == "mp3")
    {
        loadMP3(filepath, m_id);
    }
    else
    {
        Logger::getCoreLogger()->error("Unsupported audio file type: %s", extension);
    }
}

AudioBuffer::~AudioBuffer()
{
    alDeleteBuffers(1, &m_id);
}

Reference<AudioBuffer> AudioBuffer::create(const std::string& filepath)
{
    return Reference<AudioBuffer>(new AudioBuffer(filepath));
}

void AudioBuffer::loadWAV(const std::string& path, uint32_t buffer)
{
    // Load the .wav file
    drwav wav;
    if (!drwav_init_file(&wav, path.c_str(), nullptr))
    {
        Logger::getCoreLogger()->error("Could not open .wav file: %s", path);
        return;
    }

    size_t dataSize = wav.totalPCMFrameCount * wav.channels * sizeof(drwav_int16);
    drwav_int16* data = (drwav_int16*)malloc(dataSize);
    drwav_read_pcm_frames_s16(&wav, wav.totalPCMFrameCount, data);

    alBufferData(buffer, AL_FORMAT_STEREO16, data, dataSize, wav.sampleRate);

    drwav_uninit(&wav);
}

void AudioBuffer::loadMP3(const std::string& path, uint32_t buffer)
{
    // Load the .mp3 file
    drmp3 mp3;
    if (!drmp3_init_file(&mp3, path.c_str(), nullptr))
    {
        Logger::getCoreLogger()->error("Could not open .mp3 file: %s", path);
        return;
    }

    drmp3_uint64 frameCount;
    drmp3_get_mp3_and_pcm_frame_count(&mp3, nullptr, &frameCount);

    size_t dataSize = frameCount * mp3.channels * sizeof(drmp3_int16);
    drmp3_int16* data = (drmp3_int16*)malloc(dataSize);
    drmp3_read_pcm_frames_s16(&mp3, frameCount, data);

    alBufferData(buffer, AL_FORMAT_STEREO16, data, dataSize, mp3.sampleRate);

    drmp3_uninit(&mp3);
}

}