#include "keyloot.h"

#include "items/colliderfactory.h"
#include "player/player.h"
#include "resources/resourcemanager.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace Game
{

KeyLoot::KeyLoot(b2World *world, sf::Shape *shape)
    : AbstractPhysicalItem{ ColliderFactory::create<ItemType::Loot>(world, { shape }) },
      _sprite{ ResourseManager::getInstance()->getTextures(TextureType::Key).front() },
      _hint{ "Press E to pickup" }
{
    _hint.setPosition({ Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE).x,
                        Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE).y
                            - gPlayer->boundingRect().height });
    _sprite.setOrigin(Util::pointBy(_sprite.getLocalBounds(), Util::ALIGN_CENTER_STATE));
    _sprite.setScale(0.2, 0.2);
}

void KeyLoot::showHint()
{
    _isNeedShowHint = true;
}

void KeyLoot::hideHint()
{
    _isNeedShowHint = false;
}

bool KeyLoot::needDestroying() const
{
    return _needDestroy;
}

void KeyLoot::prepareDestroy()
{
    _needDestroy = true;
}

void KeyLoot::setCallback(std::function<void()> actionCallback)
{
    _actionCallback = std::move(actionCallback);
}

void KeyLoot::update(float deltatime)
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

void KeyLoot::draw(sf::RenderTarget &target, sf::RenderStates states) const
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

sf::Vector2f KeyLoot::position() const
{
    return Util::pointBy(boundingRect(), Util::ALIGN_CENTER_STATE);
}

void KeyLoot::execute()
{
    prepareDestroy();
}

} // namespace Game
