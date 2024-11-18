#pragma once

template <typename T> struct Size
{
    T width;
    T height;
};

using SizeF = Size<float>;
using SizeI = Size<int>;
using SizeU = Size<unsigned int>;
