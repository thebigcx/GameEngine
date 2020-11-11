#pragma once

#include <cmath>
#include <iostream>

#include <math/vector/Vector3.h>
#include <math/vector/Vector4.h>
#include <math/Math.h>
#include <math/matrix/Matrix.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace math
{

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

    void clear()
    {
        for (int x = 0 ; x < 4 ; x++)
        for (int y = 0 ; y < 4 ; y++)
        {
            m_cells[x][y] = static_cast<T>(0);
        }
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

    ColumnType operator[](int index) const
    {
        return m_cells[index];
    }

private:
    ColumnType m_cells[4];

    friend const T* buffer<>(const Matrix<4, 4, T>& mat);
};

typedef Matrix<4, 4, float>        Matrix4f;
typedef Matrix<4, 4, int>          Matrix4i;
typedef Matrix<4, 4, long>         Matrix4l;
typedef Matrix<4, 4, double>       Matrix4d;
typedef Matrix<4, 4, unsigned int> Matrix4u;

}