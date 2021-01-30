#include <renderer/Animation.h>

namespace Engine
{

Reference<Animation> Animation::create(Reference<Texture2D> texture)
{
    auto animation = createReference<Animation>();
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