#pragma once

namespace math
{

template<int width, int height, typename T>
class mat;

template<int width, int height, typename T>
const T* buffer(const mat<width, height, T>& val)
{
    return &(val.m_cells[0].x);
}

}