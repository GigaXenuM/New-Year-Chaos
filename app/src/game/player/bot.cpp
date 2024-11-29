#include "bot.h"

#include "resources/resourcemanager.h"

Bot::Bot(b2Body *collider, EventHandler *eventHandler)
    : Graphics::PhisicalItem(collider, { 5, 30 }, eventHandler),
      _walkAnimation{ ResourseManager::getInstance()->getTextures(TextureType::Viking_walk) }
{
}

void Bot::update(float deltatime)
{
}

void Bot::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
}

void Bot::updateAnimation(float deltatime)
{
    if (isStateActive(State::Right))
        _sprite.setScale({ _scale, _scale });
    else if (isStateActive(State::Left))
        _sprite.setScale({ -_scale, _scale });
}

void Bot::updatePosition(float deltatime)
{
}
