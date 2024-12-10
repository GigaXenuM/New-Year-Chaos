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
b2FixtureDef bulletFixDef()
{
    b2FixtureDef fixtureDef;
    fixtureDef.density = 0.01f;
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

template <> b2FixtureDef fixtureDefinition<ItemType::Entity>()
{
    return dencityFixDef();
}
template <> b2FixtureDef fixtureDefinition<ItemType::Bullet>()
{
    return bulletFixDef();
}
template <> b2FixtureDef fixtureDefinition<ItemType::Terrain>()
{
    return frictionDencityFixDef();
}
template <> b2FixtureDef fixtureDefinition<ItemType::TerrainObstacle>()
{
    return frictionDencityFixDef();
}
template <> b2FixtureDef fixtureDefinition<ItemType::Loot>()
{
    return frictionDencityFixDef();
}
template <> b2FixtureDef fixtureDefinition<ItemType::WaterZone>()
{
    return frictionDencityFixDef();
}
template <> b2FixtureDef fixtureDefinition<ItemType::DeadZone>()
{
    return frictionDencityFixDef();
}

template <> b2BodyDef bodyDefinition<ItemType::Entity>()
{
    return dynamicBodyDef();
}
template <> b2BodyDef bodyDefinition<ItemType::Bullet>()
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
template <> b2BodyDef bodyDefinition<ItemType::Loot>()
{
    return staticBodyDef();
}
template <> b2BodyDef bodyDefinition<ItemType::WaterZone>()
{
    return staticBodyDef();
}
template <> b2BodyDef bodyDefinition<ItemType::DeadZone>()
{
    return staticBodyDef();
}

} // namespace Game::ColliderFactory
