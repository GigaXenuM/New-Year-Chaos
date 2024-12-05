#include "snowball.h"

#include "items/colliderfactory.h"
#include "resources/resourcemanager.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

namespace Game
{

SnowBall::SnowBall(b2World *world, sf::Shape *shape, const AbstractPhysicalItem *shooter,
                   const Context &context)
    : PhysicalBullet{ ColliderFactory::create<ItemType::Bullet>(world, { shape }), shooter,
                      context },
      _flyAnimation{ ResourseManager::getInstance()->getTextures(TextureType::SnowBall) }
{
    _sprite.setScale({ -_scale, _scale });
}

void SnowBall::draw(sf::RenderTarget &target, sf::RenderStates states) const
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

void SnowBall::update(float deltatime)
{
    PhysicalBullet::update(deltatime);

    updatePosition();
    updateAnimation(deltatime);
}

float SnowBall::power() const
{
    return 25.f;
}

void SnowBall::updatePosition()
{
    const sf::Vector2f playerPos{ Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE) };
    _sprite.setOrigin(Util::pointBy(_sprite.getLocalBounds(), Util::ALIGN_CENTER_STATE));
    _sprite.setPosition(playerPos);

    b2Vec2 velocity = collider()->GetLinearVelocity();
    float angle = std::atan2(velocity.y, velocity.x);
    float angleInDegrees = angle * (180.0f / M_PI);
    _sprite.setRotation(angleInDegrees);
}

void SnowBall::updateAnimation(float deltatime)
{
    _flyAnimation.start(deltatime, _sprite);
}

} // namespace Game
