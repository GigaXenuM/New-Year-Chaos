#include "AxeBullet.h"

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "items/colliderfactory.h"
#include "resources/resourceManager.h"

namespace Game
{

AxeBullet::AxeBullet(b2World *world, sf::Shape *shape, const AbstractPhysicalItem *shooter,
                     const Context &context)
    : PhysicalBullet{ ColliderFactory::create<ItemType::Bullet>(world, { shape }), shooter,
                      context }
{
    _sprite.setTexture(ResourseManager::getInstance()->getTextures(TextureType::Axe)[0]);
}

void AxeBullet::update(float deltatime)
{
    PhysicalBullet::update(deltatime);
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
} // namespace Game