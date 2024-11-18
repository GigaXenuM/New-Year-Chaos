#pragma once

#include <SFML/Window/Mouse.hpp>

namespace Mouse
{
enum class Button : unsigned long long
{
    Left = sf::Mouse::Left,
    Right = sf::Mouse::Right,
    Middle = sf::Mouse::Middle,
    XButton1 = sf::Mouse::XButton1,
    XButton2 = sf::Mouse::XButton2,
    MAX_SIZE = sf::Mouse::ButtonCount
};
}
