#pragma once

template<int width, int height, typename T>
class Matrix;

template<typename T>
class Matrix<3, 3, T>
{
public:
    Matrix<3, 3, T>()
    {

    }

    Matrix<3, 3, T>(float val)
    {
        clear();
        for (int i = 0 ; i < 3 ; i++)
        {
            m_cells[i][i] = val;
        }
    }

    void setIdentity()
    {
        for (int i = 0 ; i < 3 ; i++)
        {
            m_cells[i][i] = 1.f; // Set diagonal to 1's
        }
    }

    void clear()
    {
        for (int x = 0 ; x < 3 ; x++)
        for (int y = 0 ; y < 3 ; y++)
        {
            m_cells[x][y] = 0.0f;
        }
    }

    // 4x4 matrix encapsulated vec3's
    void translate(const Vector<2, T>& vector)
    {
        m_cells[2][0] += vector.x * m_cells[0][0];
        m_cells[2][1] += vector.y * m_cells[1][1];
    }

    void scale(const Vector<2, T>& scalar)
    {
        m_cells[0][0] *= scalar.x;
        m_cells[1][1] *= scalar.y;
    }

    void rotate(T angle)
    {
        Matrix<3, 3, T> mat;

        m_cells[0][0] = cos(angle);
        m_cells[1][0] = -sin(angle);
        m_cells[0][1] = sin(angle);
        m_cells[1][1] = cos(angle);

        operator*(mat);
    }

    const T* buffer() const
    {
        return &m_cells[0][0];
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
        auto& mc = m_cells;
        auto& m2c = m2.m_cells;
        Matrix<3, 3, T> result;

        // Columns of this
        Vector3f mCol1 = getColumn(0);
        Vector3f mCol2 = getColumn(1);
        Vector3f mCol3 = getColumn(2);

        // Columns of m2
        Vector3f m2Col1 = m2.getColumn(0);
        Vector3f m2Col2 = m2.getColumn(1);
        Vector3f m2Col3 = m2.getColumn(2);

        // Multiply them together
        Vector3f dstCol1 = mCol1 * m2Col1.x + mCol2 * m2Col1.y + mCol3 * m2Col1.z;
        Vector3f dstCol2 = mCol1 * m2Col2.x + mCol2 * m2Col2.y + mCol3 * m2Col2.z;
        Vector3f dstCol3 = mCol1 * m2Col3.x + mCol2 * m2Col3.y + mCol3 * m2Col3.z;

        // Add the columns to result
        result.makeColumn(dstCol1, 0);
        result.makeColumn(dstCol2, 1);
        result.makeColumn(dstCol3, 2);

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

    Vector<3, T> getColumn(int i) const
    {
        return Vector<3, T>(m_cells[i][0], m_cells[i][1], m_cells[i][2]);
    }

    Vector<3, T> getRow(int i) const
    {
        return Vector<3, T>(m_cells[0][i], m_cells[1][i], m_cells[2][i]);
    }

    void makeColumn(const Vector<3, T>& col, int i)
    {
        m_cells[i][0] = col.x;
        m_cells[i][1] = col.y;
        m_cells[i][2] = col.z;
    }

private:
    T m_cells[3][3];
};