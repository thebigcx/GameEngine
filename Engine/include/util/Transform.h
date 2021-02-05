#pragma once

#include <iostream>

#include <maths/vector/vec2.h>
#include <maths/matrix/mat4x4.h>
#include <maths/math.h>
#include <maths/matrix/matrix_transform.h>
#include <scene/GameComponent.h>

namespace Engine
{

class Transform : public GameComponent
{
public:
    Transform()
    {

    }

    Transform(const math::vec3& translation, const math::vec3& rotation, const math::vec3& scale);

    void setTranslation(const math::vec3& translation);
    void setTranslation(float x, float y, float z);

    void setRotation(const math::vec3& rotation);
    void setRotation(float x, float y, float z);

    void setScale(const math::vec3& scale);
    void setScale(float x, float y, float z);

    void translate(const math::vec3& translation);
    void translate(float x, float y, float z);

    void rotate(const math::vec3& rotation);
    void rotate(float x, float y, float z);

    void scale(const math::vec3& scale);
    void scale(float x, float y, float z);

    void transformChanged();

    const math::vec3& getTranslation() const { return m_translation; }
    const math::vec3& getRotation() const { return m_rotation; }
    const math::vec3& getScale() const { return m_scale; }

    math::mat4 matrix() const;

private:
    math::vec3 m_translation;
    math::vec3 m_rotation;
    math::vec3 m_scale = math::vec3(1.f);
};

}