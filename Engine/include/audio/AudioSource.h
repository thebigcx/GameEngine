#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include <core/Core.h>
#include <maths/math.h>
#include <scene/GameComponent.h>

namespace Engine
{

class AudioBuffer;

class AudioSource : public GameComponent
{
public:
    enum class State
    {
        Initial,
        Playing,
        Paused,
        Stopped
    };

public:
    AudioSource();
    AudioSource(const Reference<AudioBuffer>& buffer);
    AudioSource(const std::string& buffer);
    ~AudioSource();

    void onTransformChange(const Transform& transform) override;

    void setBuffer(const Reference<AudioBuffer>& buffer);
    void setBuffer(const std::string& buffer);

    void play();
    void pause();
    void resume();
    void stop();
    void rewind();

    bool isLooped() const { return m_looped; }
    void setLooped(bool looped);

    float getGain() const { return m_gain; }
    void setGain(float gain);

    float getPitch() const { return m_pitch; }
    void setPitch(float pitch);
/*
    const math::vec3& getVelocity() const { return m_velocity; }
    void setVelocity(const math::vec3& velocity);
    void setVelocity(float x, float y, float z);

    const math::vec3& getPosition() const { return m_position; }
    void setPosition(const math::vec3& position);
    void setPosition(float x, float y, float z);
*/
    AudioSource::State getState() const;

    bool playOnStart = true;

private:
    ALuint m_id = 0;

    Reference<AudioBuffer> m_buffer;

    bool m_looped = false;

    float m_gain = 1;
    float m_pitch = 1;

    //math::vec3 m_position;
    //math::vec3 m_velocity;
};

}