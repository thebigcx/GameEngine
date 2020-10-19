#include <FGL/audio/SoundManager.h>

#include <FGL/core/Logger.h>
#include <FGL/audio/SoundListener.h>

ALCdevice* SoundManager::m_device;
ALCcontext* SoundManager::m_context;

Source SoundManager::m_source2D;
SoundBuffer SoundManager::m_buffer2D;

void SoundManager::init()
{
    m_device = alcOpenDevice(nullptr);
    m_context = alcCreateContext(m_device, nullptr);
    alcMakeContextCurrent(m_context);

    m_source2D.create();

    SoundListener::getInstance().updateALData();
}

void SoundManager::play2D(const SoundBuffer& buffer, bool loop)
{
    playFromSource(buffer, m_source2D, loop);
}

void SoundManager::play2D(const std::string& path, bool loop)
{
    // Get file extension
    std::string extension = (path.substr(path.find_last_of(".") + 1));

    if (extension == "wav")
    {
        m_buffer2D.load(SoundBuffer::FileType::WAV, path);
    }
    else if (extension == "mp3")
    {
        m_buffer2D.load(SoundBuffer::FileType::MP3, path);
    }
    else
    {
        Logger::out("Unsupported sound file type: " + extension);
        return;
    }

    play2D(m_buffer2D, loop);
}

void SoundManager::playFromSource(const SoundBuffer& buffer, const Source& source, bool loop)
{
    if (source.isPlaying())
    {
        return;
    }

    alSourcei(source.m_id, AL_BUFFER, buffer.m_id);
    alSourcei(source.m_id, AL_LOOPING, (int)loop);
    alSourcePlay(source.m_id);
}

void SoundManager::cleanUp()
{
    m_source2D.destroy();


    alcDestroyContext(m_context);
    alcCloseDevice(m_device);
}