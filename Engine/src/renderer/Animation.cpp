#include <renderer/Animation.h>

namespace Engine
{

Shared<Animation> Animation::create(Shared<Texture2D> texture)
{
    auto animation = createShared<Animation>();
    animation->m_pTexture = texture;
    return animation;
}

void Animation::pushFrame(const math::frect& frame)
{
    m_frames.push_back(frame);
}

void Animation::setFrames(const std::initializer_list<math::frect> frames)
{
    m_frames = std::vector<math::frect>(frames);
}

const math::frect& Animation::getCurrentFrame() const
{
    return m_frames[m_currentFrameIndex];
}

}