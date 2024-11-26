#pragma once

#include "alignment.h"
#include "util/enumflag.h"

#include <SFML/Graphics/Rect.hpp>

namespace Util
{
template <typename T> sf::Vector2<T> pointBy(const sf::Rect<T> &rect, Util::EnumFlag<Align> origin)
{
    T halfWidth{ rect.width / T{ 2 } };
    T halfHeight{ rect.height / T{ 2 } };

    sf::Vector2<T> result{ rect.getPosition().x + halfWidth, rect.getPosition().y + halfHeight };

    if (origin.test(Align::Left))
        result.x -= halfWidth;
    if (origin.test(Align::Right))
        result.x += halfWidth;
    if (origin.test(Align::Top))
        result.y -= halfHeight;
    if (origin.test(Align::Bottom))
        result.y += halfHeight;

    return result;
}
} // namespace Util
