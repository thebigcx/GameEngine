#include <FGL/audio/SoundBuffer.h>

#include <AL/al.h>

#include <vendor/dr_wav.h>
#include <FGL/core/Logger.h>

SoundBuffer::SoundBuffer()
{

}

SoundBuffer::SoundBuffer(unsigned int id)
    : m_id(id)
{
    
}

SoundBuffer SoundBuffer::loadWAV(const std::string& path)
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

    Logger::out(wav.bytesRemaining);

    size_t dataSize = wav.totalPCMFrameCount * wav.channels * sizeof(drwav_int16);
    drwav_int16* data = (drwav_int16*)malloc(dataSize);
    drwav_read_pcm_frames_s16(&wav, wav.totalPCMFrameCount, data);

    alBufferData(buffer, AL_FORMAT_STEREO16, data, dataSize, wav.sampleRate);

    drwav_uninit(&wav);
    
    return SoundBuffer(buffer);
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
    //destroy();
}