#pragma once

#include <cmath>
#include <iostream>

#include <math/vector/vec3.h>
#include <math/vector/vec4.h>
#include <math/math.h>
#include <math/matrix/mat.h>

namespace math
{

template<typename T>
class mat<4, 4, T>
{
private:
    typedef vec<4, T> ColumnType;

public:
    mat<4, 4, T>()
    {
        clear();
    }

    mat<4, 4, T>(float val)
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

    mat<4, 4, T> operator*(const mat<4, 4, T>& m2)
    {
        mat<4, 4, T> result;

        // Multiply them together
        vec<4, T> dstCol1 = *this[0] * m2[0].x + *this[1] * m2[0].y + *this[2] * m2[0].z + *this[3] * m2[0].w;
        vec<4, T> dstCol2 = *this[0] * m2[1].x + *this[1] * m2[1].y + *this[2] * m2[1].z + *this[3] * m2[1].w;
        vec<4, T> dstCol3 = *this[0] * m2[2].x + *this[1] * m2[2].y + *this[2] * m2[2].z + *this[3] * m2[2].w;
        vec<4, T> dstCol4 = *this[0] * m2[3].x + *this[1] * m2[3].y + *this[2] * m2[3].z + *this[3] * m2[3].w;

        // Add the columns to result
        result[0] = dstCol1;
        result[1] = dstCol2;
        result[2] = dstCol3;
        result[3] = dstCol4;

        return result;
    }

    vec<4, T> operator*(const vec<4, T>& val)
    {
        vec<4, T> result;

        result.x = getRow(0).x * val.x + getRow(0).y * val.y + getRow(0).z * val.z + getRow(0).w * val.w;
        result.y = getRow(1).x * val.x + getRow(1).y * val.y + getRow(1).z * val.z + getRow(1).w * val.w;
        result.z = getRow(2).x * val.x + getRow(2).y * val.y + getRow(2).z * val.z + getRow(2).w * val.w;
        result.w = getRow(3).x * val.x + getRow(3).y * val.y + getRow(3).z * val.z + getRow(3).w * val.w;

        return result;
    }

    vec<4, T> getRow(int i) const
    {
        return vec<4, T>(m_cells[0][i], m_cells[1][i], m_cells[2][i], m_cells[3][i]);
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

    friend const T* buffer<>(const mat<4, 4, T>& mat);
};

typedef mat<4, 4, float>        mat4;
typedef mat<4, 4, int>          imat4;
typedef mat<4, 4, long>         lmat4;
typedef mat<4, 4, double>       dmat4;
typedef mat<4, 4, unsigned int> umat4;

}