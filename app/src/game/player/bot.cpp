#include "bot.h"

#include "SFML/Graphics/ConvexShape.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "resources/resourcemanager.h"
#include "util/geometryoperation.h"

Bot::Bot(b2Body *collider, EventHandler *eventHandler)
    : Graphics::PhisicalItem(collider, { 5, 30 }, eventHandler),
      _walkAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Viking_walk) },
      _pos{ boundingRect().getPosition() }
{
    _sprite.setScale({ _scale, _scale });
    updateState(State::Right, true);
}

void Bot::update(float deltatime)
{
    Graphics::PhisicalItem::update(deltatime);

    updateAnimation(deltatime);
    updatePosition(deltatime);
}

void Bot::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sprite, states);
}

void Bot::updateAnimation(float deltatime)
{
    if (isStateActive(State::Right))
        _sprite.setScale({ _scale, _scale });
    else if (isStateActive(State::Left))
        _sprite.setScale({ -_scale, _scale });

    _walkAnimation.start(deltatime, _sprite);
}

void Bot::updatePosition(float deltatime)
{
    static bool isRightEnd{ false };
    static bool isLeftEnd{ false };

    const sf::ConvexShape shape{ Util::convertBodyToSFMLShape(collider()) };

    const sf::Vector2f playerPos{ Util::pointBy(shape.getLocalBounds(),
                                                { Align::Bottom, Align::Left, Align::Right,
                                                  Align::Top }) };

    _sprite.setOrigin(Util::pointBy(_sprite.getLocalBounds(),
                                    { Align::Bottom, Align::Left, Align::Right, Align::Top }));

    isLeftEnd = playerPos.x <= _pos.x - _moveLimit;
    isRightEnd = playerPos.x >= _pos.x + _moveLimit;

    if (isLeftEnd)
    {
        updateState(State::Right, true);
        updateState(State::Left, false);
    }
    else if (isRightEnd)
    {
        updateState(State::Left, true);
        updateState(State::Right, false);
    }

    _sprite.setPosition(playerPos);
}