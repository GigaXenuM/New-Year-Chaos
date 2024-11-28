#include "player.h"

#include "Resources/ResourceManager.h"
#include "util/geometryoperation.h"

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

Player::Player(b2Body *collider, EventHandler *eventHandler)
    : Graphics::PhisicalItem(collider, { 5, 30 }, eventHandler),
      _runAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_run) },
      _deadAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_dead) },
      _walkAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_walk) },
      _idleAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_idle) },
      _jumpAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Player_jump) }
{
    _sprite.setScale({ _scale, _scale });
}

sf::Vector2f Player::getPosition() const
{
    return _sprite.getPosition();
}

void Player::updateAnimation(float deltatime)
{
    // TODO: reset animation if keyboadrd action changed!
    // _sprite.setOrigin(_sprite.getLocalBounds().width / 2.f, _sprite.getLocalBounds().height);
    const bool isMoved{ isStateActive(State::Right) || isStateActive(State::Left) };
    if (!isMoved)
    {
        _idleAnimation.start(deltatime, _sprite);
        return;
    }

    _walkAnimation.start(deltatime, _sprite);

    if (isStateActive(State::Right))
        _sprite.setScale({ _scale, _scale });
    else if (isStateActive(State::Left))
        _sprite.setScale({ -_scale, _scale });
}

void Player::updatePosition(float deltatime)
{
    const sf::ConvexShape shape{ Util::convertBodyToSFMLShape(collider()) };
    const sf::Vector2f playerPos{ Util::pointBy(shape.getLocalBounds(),
                                                { Align::Bottom, Align::Left, Align::Right,
                                                  Align::Top }) };
    _sprite.setOrigin(Util::pointBy(_sprite.getLocalBounds(),
                                    { Align::Bottom, Align::Left, Align::Right, Align::Top }));
    _sprite.setPosition(playerPos);
}

void Player::update(float deltatime)
{
    Graphics::PhisicalItem::update(deltatime);

    updatePosition(deltatime);
    updateAnimation(deltatime);
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_sprite, states);

    sf::RectangleShape border(_sprite.getLocalBounds().getSize());
    border.setPosition(_sprite.getPosition());
    border.setOutlineColor(sf::Color::Green);
    border.setOutlineThickness(3);
    border.setFillColor(sf::Color::Transparent);
    border.setOrigin(_sprite.getOrigin());
    border.setScale(_sprite.getScale());
    target.draw(border, states);
}
