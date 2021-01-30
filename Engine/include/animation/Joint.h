#pragma once

#include <vector>
#include <string>

#include <maths/math.h>

namespace Engine
{

class Joint
{
public:
    Joint(uint32_t index, const std::string& name, const math::mat4& bindLocalTransform);

    void addChild(const Joint& child);

    const math::mat4& getAnimatedTransform() const { return m_animatedTransform; }
    const math::mat4& getInverseBindTransform() const { return m_inverseBindTransform; }

    void setAnimationTransform(const math::mat4& animationTransform);

private:
    void calcInverseBindTransform(const math::mat4& parentBindTransform);

private:
    uint32_t m_index;
    std::string m_name;
    std::vector<Joint> m_children;

    math::mat4 m_animatedTransform;
    math::mat4 m_localBindTransform;
    math::mat4 m_inverseBindTransform;
};

}