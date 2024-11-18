#pragma once

#include "SFML/Window/Keyboard.hpp"

namespace Keyboard
{

using Key = sf::Keyboard::Key;

enum class Mode : unsigned
{
    None = 0,

    Shift = 1,
    Alt = 2,
    Control = 4,

    MAX_SIZE = Control
};

inline Mode operator|(Mode a, Mode b)
{
    return static_cast<Mode>(static_cast<int>(a) | static_cast<int>(b));
}

inline Mode operator&(Mode a, Mode b)
{
    return static_cast<Mode>(static_cast<int>(a) & static_cast<int>(b));
}

inline Mode &operator|=(Mode &a, Mode b)
{
    return a = a | b;
}
} // namespace Keyboard
