#include <util/Transform.h>
#include <scene/GameObject.h>

namespace Engine
{

Transform::Transform(const math::vec3& translation, const math::vec3& rotation, const math::vec3& scale)
    : m_translation(translation), m_rotation(rotation), m_scale(scale)
{
    
}

void Transform::setTranslation(const math::vec3& translation)
{
    m_translation = translation;
    transformChanged();
}

void Transform::setTranslation(float x, float y, float z)
{
    m_translation = { x, y, z };
    transformChanged();
}

void Transform::setRotation(const math::vec3& rotation)
{
    m_rotation = rotation;
    transformChanged();
}

void Transform::setRotation(float x, float y, float z)
{
    m_rotation = { x, y, z };
    transformChanged();
}

void Transform::setScale(const math::vec3& scale)
{
    m_scale = scale;
    transformChanged();
}

void Transform::setScale(float x, float y, float z)
{
    m_scale = { x, y, z };
    transformChanged();
}

void Transform::translate(const math::vec3& translation)
{
    m_translation += translation;
    transformChanged();
}

void Transform::translate(float x, float y, float z)
{
    m_translation += math::vec3(x, y, z);
    transformChanged();
}

void Transform::rotate(const math::vec3& rotation)
{
    m_rotation += rotation;
    transformChanged();
}

void Transform::rotate(float x, float y, float z)
{
    m_rotation += math::vec3(x, y, z);
    transformChanged();
}

void Transform::scale(const math::vec3& scale)
{
    m_scale += scale;
    transformChanged();
}

void Transform::scale(float x, float y, float z)
{
    m_scale += math::vec3(x, y, z);
    transformChanged();
}

void Transform::transformChanged()
{
    m_owner->onTransformChange(*this);
}

math::mat4 Transform::matrix() const
{
    return math::translate(math::mat4(1.f), m_translation)
         * math::to_mat4(math::quat(math::radians(m_rotation)))
         * math::scale(math::mat4(1.f), m_scale);
}

}