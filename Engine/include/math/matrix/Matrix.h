#pragma once

namespace math
{

template<int width, int height, typename T>
class Matrix;

template<int width, int height, typename T>
const T* buffer(const Matrix<width, height, T>& mat)
{
    return &(mat.m_cells[0].x);
}

}