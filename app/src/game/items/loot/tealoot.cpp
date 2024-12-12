#include "tealoot.h"

#include "items/colliderfactory.h"
#include "resources/resourcemanager.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace Game
{

TeaLoot::TeaLoot(b2World *world, sf::Shape *shape)
    : AbstractPhysicalItem{ ColliderFactory::create<ItemType::Loot>(world, { shape }) },
      _sprite{ ResourseManager::getInstance()->getTextures(TextureType::Tea).front() },
      _hint{ "Press Q to pickup" }
{
    _hint.setPosition({ Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE).x,
                        Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE).y
                            - _sprite.getLocalBounds().height });
    _sprite.setOrigin(Util::pointBy(_sprite.getLocalBounds(), Util::ALIGN_CENTER_STATE));
}

void TeaLoot::showHint()
{
    _isNeedShowHint = true;
}

void TeaLoot::hideHint()
{
    _isNeedShowHint = false;
}

bool TeaLoot::needDestroying() const
{
    return _needDestroy;
}

void TeaLoot::prepareDestroy()
{
    _needDestroy = true;
}

void TeaLoot::setCallback(std::function<void()> actionCallback)
{
    _actionCallback = std::move(actionCallback);
}

void TeaLoot::update(float deltatime)
{
    _sprite.setPosition(Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE));
    if (_isNeedShowHint)
    {
        _fadeTime += deltatime;
        _hint.update(deltatime);
        if (_fadeTime >= _fadeDuration)
        {
            _fadeTime = 0.f;
            _isNeedShowHint = false;
        }
    }
}

void TeaLoot::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sprite, states);

    if (_isNeedShowHint)
        _hint.draw(target, states);

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
    _actionCallback();
}

} // namespace Game
