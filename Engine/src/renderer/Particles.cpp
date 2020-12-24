#include <renderer/Particles.h>
#include <renderer/RenderCommand.h>
#include <iostream>

namespace Engine
{

ParticleSystem::ParticleSystem(const Shared<Texture2D>& texture)
    : m_texture(texture)
{

}

void ParticleSystem::update()
{
    float dt = Time::getDelta();
    for (unsigned int i = 0; i < particles.size(); i++)
    {
        particles[i].life -= 0.001 * dt;
        particles[i].color.a = particles[i].life;

        particles[i].position += particles[i].velocity * dt * 0.05f;

        if (particles[i].life <= 0)
        {
            particles.erase(particles.begin() + i);
        }
    }
}

void ParticleSystem::render()
{
    if (particles.size() == 0)
    {
        return;
    }

    /*Renderer2D::startBatch();
    for (auto& particle : particles)
    {
        Renderer2D::renderSprite(m_texture, particle.position, math::vec2(50, 50), particle.color);
        
    }
    RenderCommand::setBlend(true);
    RenderCommand::setBlendFunction(BlendFunction::SourceAlpha, BlendFunction::One);
    Renderer2D::flushBatch();
    RenderCommand::setBlend(false);*/
}

Shared<ParticleSystem> ParticleSystem::create(const Shared<Texture2D>& texture)
{
    return createShared<ParticleSystem>(texture);
}

}