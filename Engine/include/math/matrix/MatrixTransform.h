#pragma once

#include <math/matrix/Matrix4.h>

namespace math
{

template<typename T>
Matrix<4, 4, T> identity()
{
    Matrix<4, 4, T> mat;
        
    for (int i = 0 ; i < 4 ; i++)
    {
        mat[i][i] = static_cast<T>(1);
    }

    return mat;
}

template<typename T>
Matrix<4, 4, T> translate(const Matrix<4, 4, T>& mat, const Vector<3, T>& vec)
{
    Matrix<4, 4, T> result = mat;

    result[3][0] += vec.x * mat[0][0];
    result[3][1] += vec.y * mat[1][1];
    result[3][2] += vec.z * mat[2][2];

    return result;
}

template<typename T>
Matrix<4, 4, T> scale(const Matrix<4, 4, T>& mat, const Vector<3, T>& scalar)
{
    Matrix<4, 4, T> matrix = mat;

    matrix[0] *= Vector<4, T>(scalar, static_cast<T>(1));
    matrix[1] *= Vector<4, T>(scalar, static_cast<T>(1));
    matrix[2] *= Vector<4, T>(scalar, static_cast<T>(1));

    return matrix;
}

template<typename T>
Matrix<4, 4, T> rotate(const Matrix<4, 4, T>& mat, T angle, const Vector<3, T>& axis)
{
    Matrix<4, 4, T> matrix = mat;

    T c = cos(angle);
    T s = sin(angle);
    T omc = 1.f - c;

    T x = axis.x;
    T y = axis.y;
    T z = axis.z;

    matrix[0][0] = x * x * omc + c;
    matrix[0][1] = y * x * omc + z * s;
    matrix[0][2] = x * z * omc - y * s;

    matrix[1][0] = x * y * omc - z * s;
    matrix[1][1] = y * y * omc + c;
    matrix[1][2] = y * z * omc + x * s;

    matrix[2][0] = x * z * omc + y * s;
    matrix[2][1] = y * z * omc - x * s;
    matrix[2][2] = z * z * omc + c;

    return matrix;
}

template<typename T>
Matrix<4, 4, T> ortho(T left, T right, T bottom, T top, T near, T far)
{
    Matrix<4, 4, T> mat(static_cast<T>(1));

    mat[0][0] = static_cast<T>(2) / (right - left);
    mat[1][1] = static_cast<T>(2) / (top - bottom);
    mat[2][2] = static_cast<T>(-2) / (far - near);

    mat[3][0] = -(right + left) / (right - left);
    mat[3][1] = -(top + bottom) / (top - bottom);
    mat[3][2] = -(far + near) / (far - near);

    return mat;
}

template<typename T>
Matrix<4, 4, T> perspective(T fovy, T aspect, T zNear, T zFar)
{
    Matrix<4, 4, T> result;

    result[0][0] = static_cast<T>(1) / (aspect * std::tan(fovy / static_cast<T>(2)));
    result[1][1] = static_cast<T>(1) / std::tan(fovy / static_cast<T>(2));
    result[2][2] = -(zFar + zNear) / (zFar - zNear);
    result[2][3] = static_cast<T>(1);
    result[3][2] = -(static_cast<T>(2) * zFar * zNear) / (zFar - zNear);

    return result;
}

template<typename T>
Matrix<4, 4, T> lookAt(const Vector<3, T>& pos, const Vector<3, T>& object, const Vector<3, T>& up)
{
    Matrix<4, 4, T> result = identity<T>();

    Vector<3, T> f = Vector<3, T>::normalize(object - pos);
    Vector<3, T> s = Vector<3, T>::normalize(Vector<3, T>::cross(up, f));
    Vector<3, T> u = Vector<3, T>::normalize(up);

    result[0][0] = s.x;
    result[1][0] = s.y;
    result[2][0] = s.z;

    result[0][1] = u.x;
    result[1][1] = u.y;
    result[2][1] = u.z;

    result[0][2] = f.x;
    result[1][2] = f.y;
    result[2][2] = f.z;

    result[3][0] = -Vector<3, T>::dot(s, pos);
    result[3][1] = -Vector<3, T>::dot(u, pos);
    result[3][2] = -Vector<3, T>::dot(f, pos);

    return result;
}

};