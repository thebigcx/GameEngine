#include <animation/Joint.h>

namespace Engine
{

Joint::Joint(uint32_t index, const std::string& name, const math::mat4& bindLocalTransform)
    : m_index(index), m_name(name), m_localBindTransform(bindLocalTransform)
{

}

// Adds a child joint to this joint
void Joint::addChild(const Joint& child)
{
    m_children.push_back(child);
}

// Set the joint transform
void Joint::setAnimationTransform(const math::mat4& animationTransform)
{
    m_animatedTransform = animationTransform;
}

// Called during setup, when hierarchy is already established
void Joint::calcInverseBindTransform(const math::mat4& parentBindTransform)
{
    math::mat4 bindTransform = parentBindTransform * m_localBindTransform;
    m_inverseBindTransform = math::inverse(bindTransform);

    for (auto& child : m_children)
    {
        child.calcInverseBindTransform(bindTransform);
    }
}

}