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
    Transform();
    Transform(const math::vec3& translation, const math::vec3& rotation, const math::vec3& scale);

    // Parent transform
    void onTransformChange(const Transform& transform) override;

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

    const math::vec3& getWorldTranslation() const { return m_worldTranslation; }
    const math::vec3& getWorldRotation() const { return m_worldRotation; }
    const math::vec3& getWorldScale() const { return m_worldScale; }

    math::mat4 matrix() const;
    math::mat4 worldMatrix() const;

private:
    math::vec3 m_translation;
    math::vec3 m_rotation;
    math::vec3 m_scale = math::vec3(1.f);

    math::vec3 m_worldTranslation;
    math::vec3 m_worldRotation;
    math::vec3 m_worldScale;
};

}