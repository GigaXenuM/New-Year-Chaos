#include "colliderfactory.h"

#include "itemtype.h"

namespace Game::ColliderFactory
{

namespace
{
b2FixtureDef frictionDencityFixDef()
{
    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    return fixtureDef;
}
b2FixtureDef dencityFixDef()
{
    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;

    return fixtureDef;
}

b2BodyDef staticBodyDef()
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;

    return bodyDef;
}
b2BodyDef dynamicBodyDef()
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = true;

    return bodyDef;
}
} // namespace

template <> b2FixtureDef fixtureDefinition<ItemType::Enemy>()
{
    return dencityFixDef();
}
template <> b2FixtureDef fixtureDefinition<ItemType::Player>()
{
    return dencityFixDef();
}
template <> b2FixtureDef fixtureDefinition<ItemType::SnowBall>()
{
    return dencityFixDef();
}
template <> b2FixtureDef fixtureDefinition<ItemType::Terrain>()
{
    return frictionDencityFixDef();
}
template <> b2FixtureDef fixtureDefinition<ItemType::TerrainObstacle>()
{
    return frictionDencityFixDef();
}

template <> b2BodyDef bodyDefinition<ItemType::Enemy>()
{
    return dynamicBodyDef();
}
template <> b2BodyDef bodyDefinition<ItemType::Player>()
{
    return dynamicBodyDef();
}
template <> b2BodyDef bodyDefinition<ItemType::SnowBall>()
{
    return dynamicBodyDef();
}
template <> b2BodyDef bodyDefinition<ItemType::Terrain>()
{
    return staticBodyDef();
}
template <> b2BodyDef bodyDefinition<ItemType::TerrainObstacle>()
{
    return staticBodyDef();
}

} // namespace Game::ColliderFactory
