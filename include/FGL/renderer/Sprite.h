#pragma once

#include "Texture.h"

class Sprite
{
public:
    Sprite(const Texture& texture);

private:
    const Texture* m_pTexture;
};