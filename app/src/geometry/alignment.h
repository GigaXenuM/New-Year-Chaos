#pragma once

enum class Align
{
    Left = 0x1,
    Right = 0x2,
    Top = 0x4,
    Bottom = 0x8,

    Center = Left | Right | Top | Bottom,
};

inline Align operator|(Align a, Align b)
{
    return Align(static_cast<int>(a) | static_cast<int>(b));
}

inline Align operator&(Align a, Align b)
{
    return Align(static_cast<int>(a) & static_cast<int>(b));
}

inline Align &operator|=(Align &a, Align b)
{
    return a = a | b;
}
