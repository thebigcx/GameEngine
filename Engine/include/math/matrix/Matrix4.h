#pragma once

#include <cmath>
#include <iostream>

#include <math/vector/Vector3.h>
#include <math/vector/Vector4.h>
#include <math/Math.h>
#include <math/matrix/Matrix.h>

template<typename T>
class Matrix<4, 4, T>
{
private:
    typedef Vector<4, T> ColumnType;

public:
    Matrix<4, 4, T>()
    {
        clear();
    }

    Matrix<4, 4, T>(float val)
    {
        clear();
        for (int i = 0 ; i < 4 ; i++)
        {
            m_cells[i][i] = val;
        }
    }

    static Matrix<4, 4, T> identity()
    {
        Matrix<4, 4, T> mat;
        
        for (int i = 0 ; i < 4 ; i++)
        {
            mat.m_cells[i][i] = 1.f;
        }

        return mat;
    }

    void clear()
    {
        for (int x = 0 ; x < 4 ; x++)
        for (int y = 0 ; y < 4 ; y++)
        {
            m_cells[x][y] = 0.0f;
        }
    }

    static Matrix<4, 4, T> translate(const Matrix<4, 4, T>& mat, const Vector<3, T>& vector)
    {
        Matrix<4, 4, T> matrix = mat;

        matrix[3][0] += vector.x * matrix[0][0];
        matrix[3][1] += vector.y * matrix[1][1];
        matrix[3][2] += vector.z * matrix[2][2];

        return matrix;
    }

    static Matrix<4, 4, T> scale(const Matrix<4, 4, T>& mat, const Vector<3, T>& scalar)
    {
        Matrix<4, 4, T> matrix = mat;

        matrix[0] *= Vector<4, T>(scalar, 1);
        matrix[1] *= Vector<4, T>(scalar, 1);
        matrix[2] *= Vector<4, T>(scalar, 1);

        return matrix;
    }

    static Matrix<4, 4, T> rotate(const Matrix<4, 4, T>& mat, T angle, const Vector<3, T>& axis)
    {
        Matrix<4, 4, T> matrix = mat;

        float r = Math::asRadians(angle);
        float c = cos(angle);
        float s = sin(angle);
        float omc = 1.f - c;

        float x = axis.x;
        float y = axis.y;
        float z = axis.z;

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

    // 4x4 matrix encapsulated vec3's
    Matrix<4, 4, T>& translate(const Vector<3, T>& vector)
    {
        *this = Matrix<4, 4, T>::translate(*this, vector);
        return *this;
    }

    Matrix<4, 4, T>& scale(const Vector<3, T>& scalar)
    {
        *this = Matrix<4, 4, T>::scale(*this, scalar);
        return *this;
    }

    Matrix<4, 4, T>& rotate(T angle, const Vector<3, T>& axis)
    {
        *this = Matrix<4, 4, T>::rotate(*this, angle, axis);
        return *this;
    }

    static Matrix<4, 4, float> createOrthoProjection(float left, float right, float bottom, float top, float near, float far)
    {
        Matrix<4, 4, float> mat(1.f);

        mat[0][0] = 2 / (right - left);
        mat[1][1] = 2 / (top - bottom);
        mat[2][2] = -2 / (far - near);

        mat[3][0] = -(right + left) / (right - left);
        mat[3][1] = -(top + bottom) / (top - bottom);
        mat[3][2] = -(far + near) / (far - near);

        return mat;
    }

    static Matrix<4, 4, T> createOrthoView(const Vector3f& pos)
    {
        Matrix<4, 4, T> result(1.f);

        Vector3f object(pos.x, pos.y, pos.z + 1);
        Vector3f up(0, 1, 0);

        Vector3f f = Vector3f::normalize(object - pos);
        Vector3f s = Vector3f::normalize(Vector3f::cross(up, f));
        Vector3f u = Vector3f::normalize(up);

        result[0][0] = s.x;
        result[1][0] = s.y;
        result[2][0] = s.z;

        result[0][1] = u.x;
        result[1][1] = u.y;
        result[2][1] = u.z;

        result[0][2] = f.x;
        result[1][2] = f.y;
        result[2][2] = f.z;

        result[3][0] = -Vector3f::dot(s, pos);
        result[3][1] = -Vector3f::dot(u, pos);
        result[3][2] = -Vector3f::dot(f, pos);

        return result;
    }

    const T* buffer() const
    {
        return &(m_cells[0].x);
    }

    std::string str()
    {
        std::string s;
        for (int x = 0 ; x < 4 ; x++)
        {
            for (int y = 0 ; y < 4 ; y++)
            {
                s.append(std::to_string(m_cells[y][x]));
                s.append(", ");
            }
            s.append("\n");
        }

        return s;
    }

    Matrix<4, 4, T> operator*(const Matrix<4, 4, T>& m2)
    {
        Matrix<4, 4, T> result;

        // Multiply them together
        Vector<4, T> dstCol1 = *this[0] * m2[0].x + *this[1] * m2[0].y + *this[2] * m2[0].z + *this[3] * m2[0].w;
        Vector<4, T> dstCol2 = *this[0] * m2[1].x + *this[1] * m2[1].y + *this[2] * m2[1].z + *this[3] * m2[1].w;
        Vector<4, T> dstCol3 = *this[0] * m2[2].x + *this[1] * m2[2].y + *this[2] * m2[2].z + *this[3] * m2[2].w;
        Vector<4, T> dstCol4 = *this[0] * m2[3].x + *this[1] * m2[3].y + *this[2] * m2[3].z + *this[3] * m2[3].w;

        // Add the columns to result
        result[0] = dstCol1;
        result[1] = dstCol2;
        result[2] = dstCol3;
        result[3] = dstCol4;

        return result;
    }

    Vector<4, T> operator*(const Vector<4, T>& vec)
    {
        Vector<4, T> result;

        result.x = getRow(0).x * vec.x + getRow(0).y * vec.y + getRow(0).z * vec.z + getRow(0).w * vec.w;
        result.y = getRow(1).x * vec.x + getRow(1).y * vec.y + getRow(1).z * vec.z + getRow(1).w * vec.w;
        result.z = getRow(2).x * vec.x + getRow(2).y * vec.y + getRow(2).z * vec.z + getRow(2).w * vec.w;
        result.w = getRow(3).x * vec.x + getRow(3).y * vec.y + getRow(3).z * vec.z + getRow(3).w * vec.w;

        return result;
    }

    Vector<4, T> getRow(int i) const
    {
        return Vector<4, T>(m_cells[0][i], m_cells[1][i], m_cells[2][i], m_cells[3][i]);
    }

    ColumnType& operator[](int index)
    {
        return m_cells[index];
    }

private:
    ColumnType m_cells[4];
};

typedef Matrix<4, 4, float>        Matrix4f;
typedef Matrix<4, 4, int>          Matrix4i;
typedef Matrix<4, 4, long>         Matrix4l;
typedef Matrix<4, 4, double>       Matrix4d;
typedef Matrix<4, 4, unsigned int> Matrix4u;