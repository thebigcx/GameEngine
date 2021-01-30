#pragma once

#include <vector>

#include <maths/vector/vec2.h>
#include <maths/vector/vec4.h>
#include <renderer/Texture2D.h>
#include <renderer/Renderer2D.h>
#include <util/Time.h>
#include <core/Core.h>

namespace Engine
{

struct Particle
{
    math::vec2 position, velocity;
    math::vec4 color = math::vec4(1.f);
    float life;

    Particle() 
        : life(1.f)
    {}
};

class ParticleSystem
{
public:
    ParticleSystem(const Reference<Texture2D>& texture);
    std::vector<Particle> particles;

    void update();
    void render();

    static Reference<ParticleSystem> create(const Reference<Texture2D>& texture);

private:
    Reference<Texture2D> m_texture;
};

}