#include "tealoot.h"

#include "items/colliderfactory.h"
#include "resources/resourcemanager.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>

namespace Game
{

TeaLoot::TeaLoot(b2World *world, sf::Shape *shape)
    : AbstractPhysicalItem{ ColliderFactory::create<ItemType::Loot>(world, { shape }) },
      _sprite{ ResourseManager::getInstance()->getTextures(TextureType::Tea).front() }
{
    _sprite.setOrigin(Util::pointBy(_sprite.getLocalBounds(), Util::ALIGN_CENTER_STATE));
}

void TeaLoot::update(float deltatime)
{
    _sprite.setPosition(Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE));
}

void TeaLoot::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sprite, states);

    auto shape{ Util::convertBodyToSFMLShape(collider()) };

    sf::RectangleShape border(shape.getLocalBounds().getSize());
    border.setPosition(shape.getPosition());
    border.setOutlineColor(sf::Color::Magenta);
    border.setOutlineThickness(10);
    border.setFillColor(sf::Color::Transparent);
    border.setOrigin(shape.getOrigin());
    border.setScale(shape.getScale());
    target.draw(border, states);
}

sf::Vector2f TeaLoot::position() const
{
    return Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE);
}

void TeaLoot::execute()
{
    std::cout << "TeaLoot: action executed." << std::endl;
}

} // namespace Game
