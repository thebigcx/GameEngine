#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include "SoundBuffer.h"

class Source
{
public:
    Source();
    ~Source();

    void create();

    void destroy();

    bool isPlaying() const;

    friend class SoundManager;

private:
    unsigned int m_id;
};