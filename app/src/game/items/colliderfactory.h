#pragma once

#include "util/geometryoperation.h"

#include <SFML/Graphics/Shape.hpp>

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_world.h>

#include <vector>

namespace sf
{
class Shape;
}

class b2Body;
class b2World;
class b2FixtureDef;
class b2BodyDef;

namespace Game
{

enum class ItemType;

namespace ColliderFactory
{

template <ItemType> b2FixtureDef fixtureDefinition();
template <ItemType> b2BodyDef bodyDefinition();

template <ItemType TYPE> b2Body *create(b2World *world, const std::vector<sf::Shape *> &shapes)
{
    const auto bodyDef{ bodyDefinition<TYPE>() };
    auto fixDef{ fixtureDefinition<TYPE>() };

    b2Body *body{ world->CreateBody(&bodyDef) };
    for (const sf::Shape *shape : shapes)
        Util::createComplexFixture(body, shape, &fixDef);

    return body;
}

}; // namespace ColliderFactory

} // namespace Game
