#pragma once

#include <math/matrix/mat4.h>

namespace math
{

template<typename T>
mat<4, 4, T> identity()
{
    mat<4, 4, T> mat;
        
    for (int i = 0 ; i < 4 ; i++)
    {
        mat[i][i] = static_cast<T>(1);
    }

    return mat;
}

template<typename T>
mat<4, 4, T> translate(const mat<4, 4, T>& matrix, const vec<3, T>& vec)
{
    mat<4, 4, T> result = matrix;

    result[3][0] += vec.x * matrix[0][0];
    result[3][1] += vec.y * matrix[1][1];
    result[3][2] += vec.z * matrix[2][2];

    return result;
}

template<typename T>
mat<4, 4, T> scale(const mat<4, 4, T>& matrix, const vec<3, T>& scalar)
{
    mat<4, 4, T> result = matrix;

    result[0] *= vec<4, T>(scalar, static_cast<T>(1));
    result[1] *= vec<4, T>(scalar, static_cast<T>(1));
    result[2] *= vec<4, T>(scalar, static_cast<T>(1));

    return result;
}

template<typename T>
mat<4, 4, T> rotate(const mat<4, 4, T>& matrix, T angle, const vec<3, T>& axis)
{
    mat<4, 4, T> result = matrix;

    T c = cos(angle);
    T s = sin(angle);
    T omc = 1.f - c;

    T x = axis.x;
    T y = axis.y;
    T z = axis.z;

    result[0][0] = x * x * omc + c;
    result[0][1] = y * x * omc + z * s;
    result[0][2] = x * z * omc - y * s;

    result[1][0] = x * y * omc - z * s;
    result[1][1] = y * y * omc + c;
    result[1][2] = y * z * omc + x * s;

    result[2][0] = x * z * omc + y * s;
    result[2][1] = y * z * omc - x * s;
    result[2][2] = z * z * omc + c;

    return result;
}

template<typename T>
mat<4, 4, T> ortho(T left, T right, T bottom, T top, T near, T far)
{
    mat<4, 4, T> mat(static_cast<T>(1));

    mat[0][0] = static_cast<T>(2) / (right - left);
    mat[1][1] = static_cast<T>(2) / (top - bottom);
    mat[2][2] = static_cast<T>(-2) / (far - near);

    mat[3][0] = -(right + left) / (right - left);
    mat[3][1] = -(top + bottom) / (top - bottom);
    mat[3][2] = -(far + near) / (far - near);

    return mat;
}

template<typename T>
mat<4, 4, T> perspective(T fovy, T aspect, T zNear, T zFar)
{
    mat<4, 4, T> result;

    result[0][0] = static_cast<T>(1) / (aspect * std::tan(fovy / static_cast<T>(2)));
    result[1][1] = static_cast<T>(1) / std::tan(fovy / static_cast<T>(2));
    result[2][2] = -(zFar + zNear) / (zFar - zNear);
    result[2][3] = static_cast<T>(1);
    result[3][2] = -(static_cast<T>(2) * zFar * zNear) / (zFar - zNear);

    return result;
}

template<typename T>
mat<4, 4, T> lookAt(const vec<3, T>& pos, const vec<3, T>& object, const vec<3, T>& up)
{
    mat<4, 4, T> result = identity<T>();

    vec<3, T> f = vec<3, T>::normalize(object - pos);
    vec<3, T> s = vec<3, T>::normalize(vec<3, T>::cross(up, f));
    vec<3, T> u = vec<3, T>::normalize(up);

    result[0][0] = s.x;
    result[1][0] = s.y;
    result[2][0] = s.z;

    result[0][1] = u.x;
    result[1][1] = u.y;
    result[2][1] = u.z;

    result[0][2] = f.x;
    result[1][2] = f.y;
    result[2][2] = f.z;

    result[3][0] = -vec<3, T>::dot(s, pos);
    result[3][1] = -vec<3, T>::dot(u, pos);
    result[3][2] = -vec<3, T>::dot(f, pos);

    return result;
}

};