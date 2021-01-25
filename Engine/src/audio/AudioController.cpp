#include <audio/AudioController.h>
#include <audio/AudioListener.h>

namespace Engine
{

AudioController::AudioController()
    : m_context(nullptr), m_device(nullptr)
{

}

AudioController::~AudioController()
{

}

AudioController* AudioController::getInstance()
{
    static AudioController controller;
    return &controller;
}

void AudioController::initialize()
{
    m_device = alcOpenDevice(nullptr);
    m_context = alcCreateContext(m_device, nullptr);
    alcMakeContextCurrent(m_context);
}

void AudioController::finalize()
{
    alcDestroyContext(m_context);
    alcCloseDevice(m_device);
}

}