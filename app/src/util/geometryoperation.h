#pragma once

#include "alignment.h"
#include "util/enumflag.h"

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <box2d/b2_body.h>
#include <box2d/b2_math.h>
#include <box2d/b2_types.h>

struct b2Color;
struct b2FixtureDef;

namespace sf
{
class Color;
class Shape;
class ConvexShape;
} // namespace sf

namespace Util
{

constexpr Util::EnumFlag<Align> ALIGN_CENTER_STATE{ Align::Left, Align::Bottom, Align::Right,
                                                    Align::Top };

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

sf::Vector2f b2VecToSFVec(const b2Vec2 &vector);
b2Vec2 sfVecToB2Vec(sf::Vector2f vector);

sf::Color glColorToSFML(const b2Color &color, uint8 alpha = 255);

void createComplexFixture(b2Body *body, const sf::Shape *sfShape, b2FixtureDef *fixtureDefinition);

sf::ConvexShape convertBodyToSFMLShape(const b2Body *body);

template <typename T> float distance(const sf::Vector2<T> &vec2)
{
    return sqrt(vec2.x * vec2.x + vec2.y * vec2.y);
}

template <typename ToType, typename FromType>
sf::Vector2<ToType> castSfVector(const sf::Vector2<FromType> &vector)
{
    return { static_cast<ToType>(vector.x), static_cast<ToType>(vector.y) };
}

template <typename ToType, typename FromType>
sf::Rect<ToType> castSfRect(const sf::Rect<FromType> &rect)
{
    return { castSfVector<ToType>(rect.getPosition()), castSfVector<ToType>(rect.getSize()) };
}

} // namespace Util
