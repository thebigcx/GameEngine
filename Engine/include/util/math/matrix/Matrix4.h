#pragma once

#include <util/math/vector/Vector3.h>
#include <util/math/vector/Vector4.h>

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
    void translate(const Vector<3, T>& vector)
    {
        m_cells[3][0] += vector.x * m_cells[0][0];
        m_cells[3][1] += vector.y * m_cells[1][1];
        m_cells[3][2] += vector.z * m_cells[2][2];
    }

    void scale(const Vector<3, T>& scalar)
    {
        m_cells[0][0] *= scalar.x;
        m_cells[1][1] *= scalar.y;
        m_cells[2][2] *= scalar.z;
    }

    void rotate(T angle, const Vector<3, T>& axis)
    {
        Matrix<4, 4, T> mat;

        if (axis.z == 1)
        {
            m_cells[0][0] = cos(angle);
            m_cells[1][0] = -sin(angle);
            m_cells[0][1] = sin(angle);
            m_cells[1][1] = cos(angle);
        }
        else if (axis.x == 1)
        {
            m_cells[1][1] = cos(angle);
            m_cells[2][1] = -sin(angle);
            m_cells[1][2] = sin(angle);
            m_cells[2][2] = cos(angle);
        }
        else if (axis.y == 1)
        {
            m_cells[0][0] = cos(angle);
            m_cells[2][0] = sin(angle);
            m_cells[0][2] = -sin(angle);
            m_cells[2][2] = cos(angle);
        }

        operator*(mat);
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
                s.append(std::to_string(m_cells[x][y]));
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