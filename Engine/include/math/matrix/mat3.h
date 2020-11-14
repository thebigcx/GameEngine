#pragma once

#include <math/matrix/mat.h>

namespace math
{

template<typename T>
class mat<3, 3, T>
{
private:
    typedef vec<3, T> ColumnType;

public:
    mat<3, 3, T>()
    {
        clear();
    }

    mat<3, 3, T>(float val)
    {
        clear();
        for (int i = 0 ; i < 3 ; i++)
        {
            m_cells[i][i] = val;
        }
    }

    static mat<3, 3, T> identity()
    {
        mat<3, 3, T> mat;

        for (int i = 0 ; i < 3 ; i++)
        {
            mat[i][i] = 1.f; // Set diagonal to 1's
        }

        return mat;
    }

    void clear()
    {
        for (int x = 0 ; x < 3 ; x++)
        for (int y = 0 ; y < 3 ; y++)
        {
            m_cells[x][y] = 0.0f;
        }
    }
    
    static mat<3, 3, T> translate(const mat<3, 3, T>& val, const vec<2, T>& vector)
    {
        mat<3, 3, T> result = val;

        result[2][0] += vector.x * result[0][0];
        result[2][1] += vector.y * result[1][1];

        return result;
    }

    static mat<3, 3, T> rotate(const mat<3, 3, T>& val, T angle)
    {
        mat<3, 3, T> result = val;

        result[0][0] *= cos(angle);
        result[1][0] *= -sin(angle);
        result[0][1] *= sin(angle);
        result[1][1] *= cos(angle);

        return result;
    }

    static mat<3, 3, T> scale(const mat<3, 3, T>& matrix, const vec<2, T>& scalar)
    {
        mat<3, 3, T> result = matrix;

        result[0] *= vec<3, T>(scalar, 1);
        result[1] *= vec<3, T>(scalar, 1);

        return result;
    }

    // 4x4 mat encapsulated vec3's
    mat<3, 3, T>& translate(const vec<2, T>& vector)
    {
        *this = mat<3, 3, T>::translate(*this, vector);
        return *this;
    }

    mat<3, 3, T>& scale(const vec<2, T>& scalar)
    {
        *this = mat<3, 3, T>::scale(*this, scalar);
        return *this;
    }

    mat<3, 3, T>& rotate(T angle)
    {
        *this = mat<3, 3, T>::rotate(*this, angle);
        return *this;
    }

    std::string str()
    {
        std::string s;
        for (int x = 0 ; x < 3 ; x++)
        {
            for (int y = 0 ; y < 3 ; y++)
            {
                s.append(std::to_string(m_cells[x][y]));
                s.append(", ");
            }
            s.append("\n");
        }

        return s;
    }

    mat<3, 3, T> operator*(const mat<3, 3, T>& m2)
    {
        mat<3, 3, T> result;

        // Multiply them together
        result[0] = m_cells[0] * m2[0].x + m_cells[1] * m2[0].y + m_cells[2] * m2[0].z;
        result[1] = m_cells[0] * m2[1].x + m_cells[1] * m2[1].y + m_cells[2] * m2[1].z;
        result[2] = m_cells[0] * m2[2].x + m_cells[1] * m2[2].y + m_cells[2] * m2[2].z;

        return result;
    }

    vec<3, T> operator*(const vec<3, T>& val)
    {
        vec<3, T> result;

        result.x = getRow(0).x * val.x + getRow(0).y * val.y + getRow(0).z * val.z;
        result.y = getRow(1).x * val.x + getRow(1).y * val.y + getRow(1).z * val.z;
        result.z = getRow(2).x * val.x + getRow(2).y * val.y + getRow(2).z * val.z;

        return result;
    }

    vec<3, T> getRow(int i) const
    {
        return vec<3, T>(m_cells[0][i], m_cells[1][i], m_cells[2][i]);
    }

private:
    ColumnType m_cells[3];

    friend const T* buffer<>(const mat<3, 3, T>& mat);
};

typedef mat<3, 3, float>        mat3;
typedef mat<3, 3, int>          imat3;
typedef mat<3, 3, long>         lmat3;
typedef mat<3, 3, double>       dmat3;
typedef mat<3, 3, unsigned int> umat3;

}