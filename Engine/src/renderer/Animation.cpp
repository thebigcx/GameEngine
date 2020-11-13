#include <renderer/Animation.h>

Shared<Animation> Animation::create(Shared<Texture2D> texture)
{
    auto animation = createShared<Animation>();
    animation->m_pTexture = texture;
    return animation;
}

void Animation::pushFrame(const FloatRect& frame)
{
    m_frames.push_back(frame);
}

void Animation::setFrames(const std::initializer_list<FloatRect> frames)
{
    m_frames = std::vector<FloatRect>(frames);
}

const FloatRect& Animation::getCurrentFrame() const
{
    return m_frames[m_currentFrameIndex];
}