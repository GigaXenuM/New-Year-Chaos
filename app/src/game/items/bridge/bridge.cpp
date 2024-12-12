#include "bridge.h"

#include "items/colliderfactory.h"
#include "resources/resourcemanager.h"

#include "SFML/Graphics/RenderTarget.hpp"
#include <box2d/b2_polygon_shape.h>

namespace Game
{

Bridge::Bridge(b2World *world, sf::Shape *shape)
    : AbstractPhysicalItem{ ColliderFactory::create<ItemType::Terrain>(world, { shape }) },
      _sprite{ ResourseManager::getInstance()->getTextures(TextureType::Bridge).front() }
{
    constexpr float scale{ 0.5f };
    _sprite.setScale(scale, scale);
}

bool Bridge::needDestroying() const
{
    return false;
}

void Bridge::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sprite, states);
}

void Bridge::update(float deltatime)
{
    const sf::Vector2f position{ Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE) };
    _sprite.setOrigin(Util::pointBy(_sprite.getLocalBounds(), Util::ALIGN_CENTER_STATE));
    _sprite.setPosition(position);

    constexpr float rotationOffset = -90.f;
    constexpr float RAD_TO_DEG = 180.f / b2_pi;
    float angleInDegrees = collider()->GetAngle() * RAD_TO_DEG;
    _sprite.setRotation(angleInDegrees + rotationOffset);
}

sf::Vector2f Bridge::position() const
{
    return Util::pointBy(boundingRect(), { Align::Left, Align::Bottom });
}

void Bridge::execute()
{
    collider()->SetType(b2BodyType::b2_dynamicBody);
    _type = ItemType::Terrain;
}

} // namespace Game
