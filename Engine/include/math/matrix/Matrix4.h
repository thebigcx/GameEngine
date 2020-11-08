#pragma once

#include <cmath>
#include <iostream>

#include <math/vector/Vector3.h>
#include <math/vector/Vector4.h>
#include <math/Math.h>

template<int width, int height, typename T>
class Matrix;

template<typename T>
class Matrix<4, 4, T>
{
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

    void setIdentity()
    {
        for (int i = 0 ; i < 4 ; i++)
        {
            m_cells[i][i] = 1.f; // Set diagonal to 1's
        }
    }

    void clear()
    {
        for (int x = 0 ; x < 4 ; x++)
        for (int y = 0 ; y < 4 ; y++)
        {
            m_cells[x][y] = 0.0f;
        }
    }

    // 4x4 matrix encapsulated vec3's
    Matrix<4, 4, T>& translate(const Vector<3, T>& vector)
    {
        m_cells[3][0] += vector.x * m_cells[0][0];
        m_cells[3][1] += vector.y * m_cells[1][1];
        m_cells[3][2] += vector.z * m_cells[2][2];
        return *this;
    }

    void scale(const Vector<3, T>& scalar)
    {
        makeColumn(Vector<4, T>(scalar, 1) * getColumn(0), 0);
        makeColumn(Vector<4, T>(scalar, 1) * getColumn(1), 1);
        makeColumn(Vector<4, T>(scalar, 1) * getColumn(2), 2);
    }

    void rotate(T angle, const Vector<3, T>& axis)
    {
        float r = Math::asRadians(angle);
        float c = cos(angle);
        float s = sin(angle);
        float omc = 1.f - c;

        float x = axis.x;
        float y = axis.y;
        float z = axis.z;

        m_cells[0][0] = x * x * omc + c;
        m_cells[0][1] = y * x * omc + z * s;
        m_cells[0][2] = x * z * omc - y * s;

        m_cells[1][0] = x * y * omc - z * s;
        m_cells[1][1] = y * y * omc + c;
        m_cells[1][2] = y * z * omc + x * s;

        m_cells[2][0] = x * z * omc + y * s;
        m_cells[2][1] = y * z * omc - x * s;
        m_cells[2][2] = z * z * omc + c;
    }

    static Matrix<4, 4, float> createOrthoProjection(float left, float right, float bottom, float top, float near, float far)
    {
        Matrix<4, 4, float> mat(1.f);

        mat.m_cells[0][0] = 2 / (right - left);
        mat.m_cells[1][1] = 2 / (top - bottom);
        mat.m_cells[2][2] = -2 / (far - near);

        mat.m_cells[3][0] = -(right + left) / (right - left);
        mat.m_cells[3][1] = -(top + bottom) / (top - bottom);
        mat.m_cells[3][2] = -(far + near) / (far - near);

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

        result.m_cells[0][0] = s.x;
        result.m_cells[1][0] = s.y;
        result.m_cells[2][0] = s.z;

        result.m_cells[0][1] = u.x;
        result.m_cells[1][1] = u.y;
        result.m_cells[2][1] = u.z;

        result.m_cells[0][2] = f.x;
        result.m_cells[1][2] = f.y;
        result.m_cells[2][2] = f.z;

        result.m_cells[3][0] = -Vector3f::dot(s, pos);
        result.m_cells[3][1] = -Vector3f::dot(u, pos);
        result.m_cells[3][2] = -Vector3f::dot(f, pos);

        return result;
    }

    const T* buffer() const
    {
        return &m_cells[0][0];
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
        auto& mc = m_cells;
        auto& m2c = m2.m_cells;
        Matrix<4, 4, T> result;

        // Columns of this
        Vector4f mCol1 = getColumn(0);
        Vector4f mCol2 = getColumn(1);
        Vector4f mCol3 = getColumn(2);
        Vector4f mCol4 = getColumn(3);

        // Columns of m2
        Vector4f m2Col1 = m2.getColumn(0);
        Vector4f m2Col2 = m2.getColumn(1);
        Vector4f m2Col3 = m2.getColumn(2);
        Vector4f m2Col4 = m2.getColumn(3);

        // Multiply them together
        Vector4f dstCol1 = mCol1 * m2Col1.x + mCol2 * m2Col1.y + mCol3 * m2Col1.z + mCol4 * m2Col1.w;
        Vector4f dstCol2 = mCol1 * m2Col2.x + mCol2 * m2Col2.y + mCol3 * m2Col2.z + mCol4 * m2Col2.w;
        Vector4f dstCol3 = mCol1 * m2Col3.x + mCol2 * m2Col3.y + mCol3 * m2Col3.z + mCol4 * m2Col3.w;
        Vector4f dstCol4 = mCol1 * m2Col4.x + mCol2 * m2Col4.y + mCol3 * m2Col4.z + mCol4 * m2Col4.w;

        // Add the columns to result
        result.makeColumn(dstCol1, 0);
        result.makeColumn(dstCol2, 1);
        result.makeColumn(dstCol3, 2);
        result.makeColumn(dstCol4, 3);

        return result;
    }

    Vector4f operator*(const Vector4f& vec)
    {
        Vector<4, T> result;

        result.x = getRow(0).x * vec.x + getRow(0).y * vec.y + getRow(0).z * vec.z + getRow(0).w * vec.w;
        result.y = getRow(1).x * vec.x + getRow(1).y * vec.y + getRow(1).z * vec.z + getRow(1).w * vec.w;
        result.z = getRow(2).x * vec.x + getRow(2).y * vec.y + getRow(2).z * vec.z + getRow(2).w * vec.w;
        result.w = getRow(3).x * vec.x + getRow(3).y * vec.y + getRow(3).z * vec.z + getRow(3).w * vec.w;

        return result;
    }

    Vector<4, T> getColumn(int i) const
    {
        return Vector<4, T>(m_cells[i][0], m_cells[i][1], m_cells[i][2], m_cells[i][3]);
    }

    Vector<4, T> getRow(int i) const
    {
        return Vector<4, T>(m_cells[0][i], m_cells[1][i], m_cells[2][i], m_cells[3][i]);
    }

    void makeColumn(const Vector<4, T>& col, int i)
    {
        m_cells[i][0] = col.x;
        m_cells[i][1] = col.y;
        m_cells[i][2] = col.z;
        m_cells[i][3] = col.w;
    }

private:
    T m_cells[4][4];
};

typedef Matrix<4, 4, float>        Matrix4f;
typedef Matrix<4, 4, int>          Matrix4i;
typedef Matrix<4, 4, long>         Matrix4l;
typedef Matrix<4, 4, double>       Matrix4d;
typedef Matrix<4, 4, unsigned int> Matrix4u;