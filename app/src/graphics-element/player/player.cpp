#include "player.h"

#include "SFML/Graphics/RenderTarget.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

#include "event/keyevents/keypressevent.h"

Player::Player(EventHandler *eventHandler) : Graphics::AbstractItem(eventHandler)
{
}

RectF Player::globalRect() const
{
    return RectF{ { 0, 0 }, { 100, 100 } };
}

RectF Player::localRect() const
{
    return {};
}

PointF Player::center() const
{
    return {};
}

void Player::setPos(PointF position)
{
}

void Player::setOrigin(Align origin)
{
}

void Player::keyPressEvent(KeyPressEvent *event)
{
    if (event->key() == sf::Keyboard::Right)
    {
    }
    if (event->key() == sf::Keyboard::Left)
    {
    }
    if (event->key() == sf::Keyboard::Down)
    {
    }
    if (event->key() == sf::Keyboard::Space)
    {
    }
}

void Player::keyReleaseEvent(KeyReleaseEvent *event)
{
}

void Player::update(float deltatime)
{
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(100.f, 100.f));
    rectangle.setFillColor(sf::Color::Red);

    rectangle.setPosition(0, 0);

    target.draw(rectangle, states);
}
