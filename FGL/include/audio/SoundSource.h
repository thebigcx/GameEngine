#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include "SoundBuffer.h"

class SoundSource
{
public:
    SoundSource();
    ~SoundSource();

    void create();

    void destroy();

    bool isPlaying() const;

    friend class SoundEngine;

private:
    unsigned int m_id;
};