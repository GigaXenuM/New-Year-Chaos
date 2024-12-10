#include "axebullet.h"

#include "items/colliderfactory.h"
#include "resources/resourcemanager.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <cmath>

namespace Game
{

AxeBullet::AxeBullet(b2World *world, sf::Shape *shape, const AbstractPhysicalItem *shooter,
                     const Context &context)
    : PhysicalBullet{ ColliderFactory::create<ItemType::Bullet>(world, { shape }), shooter,
                      context }
{
    _sprite.setTexture(ResourseManager::getInstance()->getTextures(TextureType::Axe)[0]);
    _sprite.setScale(0.2, 0.2);
}

void AxeBullet::update(float deltatime)
{
    PhysicalBullet::update(deltatime);
    updatePosition(deltatime);
}

float AxeBullet::power() const
{
    return 30.f;
}

void AxeBullet::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sprite, states);

    sf::RectangleShape border(_sprite.getLocalBounds().getSize());
    border.setPosition(_sprite.getPosition());
    border.setOutlineColor(sf::Color::Red);
    border.setOutlineThickness(10);
    border.setFillColor(sf::Color::Transparent);
    border.setOrigin(_sprite.getOrigin());
    border.setScale(_sprite.getScale());
    target.draw(border, states);
}

void AxeBullet::updatePosition(float deltatime)
{
    // const sf::Vector2f playerPos{ Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE) };
    // _sprite.setOrigin(Util::pointBy(_sprite.getLocalBounds(), Util::ALIGN_CENTER_STATE));
    // _sprite.setPosition(playerPos);

    // b2Vec2 velocity = collider()->GetLinearVelocity();
    // float angle = std::atan2(velocity.y, velocity.x);
    // float angleInDegrees = angle * (180.0f / M_PI);
    // _sprite.setRotation(angleInDegrees);
}
} // namespace Game
