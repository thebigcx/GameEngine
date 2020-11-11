#pragma once

#include <math/matrix/Matrix.h>

namespace math
{

template<typename T>
class Matrix<3, 3, T>
{
private:
    typedef Vector<3, T> ColumnType;

public:
    Matrix<3, 3, T>()
    {
        clear();
    }

    Matrix<3, 3, T>(float val)
    {
        clear();
        for (int i = 0 ; i < 3 ; i++)
        {
            m_cells[i][i] = val;
        }
    }

    static Matrix<3, 3, T> identity()
    {
        Matrix<3, 3, T> mat;

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
    
    static Matrix<3, 3, T> translate(const Matrix<3, 3, T>& mat, const Vector<2, T>& vector)
    {
        Matrix<3, 3, T> result = mat;

        result[2][0] += vector.x * result[0][0];
        result[2][1] += vector.y * result[1][1];

        return result;
    }

    static Matrix<3, 3, T> rotate(const Matrix<3, 3, T>& mat, T angle)
    {
        Matrix<3, 3, T> result;

        result[0][0] *= cos(angle);
        result[1][0] *= -sin(angle);
        result[0][1] *= sin(angle);
        result[1][1] *= cos(angle);

        return result;
    }

    static Matrix<3, 3, T> scale(const Matrix<3, 3, T>& mat, const Vector<2, T>& scalar)
    {
        Matrix<3, 3, T> matrix = mat;

        matrix[0] *= Vector<3, T>(scalar, 1);
        matrix[1] *= Vector<3, T>(scalar, 1);

        return matrix;
    }

    // 4x4 matrix encapsulated vec3's
    Matrix<3, 3, T>& translate(const Vector<2, T>& vector)
    {
        *this = Matrix<3, 3, T>::translate(*this, vector);
        return *this;
    }

    Matrix<3, 3, T>& scale(const Vector<2, T>& scalar)
    {
        *this = Matrix<3, 3, T>::scale(*this, scalar);
        return *this;
    }

    Matrix<3, 3, T>& rotate(T angle)
    {
        *this = Matrix<3, 3, T>::rotate(*this, angle);
        return *this;
    }

    static const T* buffer(const Matrix<3, 3, T>& mat)
    {
        return &(mat[0].x);
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

    Matrix<3, 3, T> operator*(const Matrix<3, 3, T>& m2)
    {
        Matrix<3, 3, T> result;

        // Multiply them together
        result[0] = m_cells[0] * m2[0].x + m_cells[1] * m2[0].y + m_cells[2] * m2[0].z;
        result[1] = m_cells[0] * m2[1].x + m_cells[1] * m2[1].y + m_cells[2] * m2[1].z;
        result[2] = m_cells[0] * m2[2].x + m_cells[1] * m2[2].y + m_cells[2] * m2[2].z;

        return result;
    }

    Vector3f operator*(const Vector3f& vec)
    {
        Vector<3, T> result;

        result.x = getRow(0).x * vec.x + getRow(0).y * vec.y + getRow(0).z * vec.z;
        result.y = getRow(1).x * vec.x + getRow(1).y * vec.y + getRow(1).z * vec.z;
        result.z = getRow(2).x * vec.x + getRow(2).y * vec.y + getRow(2).z * vec.z;

        return result;
    }

    Vector<3, T> getRow(int i) const
    {
        return Vector<3, T>(m_cells[0][i], m_cells[1][i], m_cells[2][i]);
    }

private:
    ColumnType m_cells[3];
};

}