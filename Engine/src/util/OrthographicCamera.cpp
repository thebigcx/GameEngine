#include <util/OrthographicCamera.h>

OrthographicCamera::OrthographicCamera(const Vector2f& position)
    : m_position(position)
{

}

void OrthographicCamera::setPosition(const Vector2f& position)
{
    m_position = position;
}

void OrthographicCamera::translate(const Vector2f& vec)
{
    m_position += vec;
}

Matrix4f OrthographicCamera::getViewMatrix() const
{
    Matrix4f mat(1.f);
    mat.translate(Vector3f(-m_position.x, -m_position.y, 0));

    return mat;
}