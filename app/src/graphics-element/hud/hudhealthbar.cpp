#include "hudhealthbar.h"
#include "SFML/Graphics/Texture.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include <Resources/ResourceManager.h>
#include <SFML/Graphics/Sprite.hpp>

HUDHealthBar::HUDHealthBar(sf::View *view) : Graphics::AbstractItem(nullptr), _gameView(view)
{
    setup();
}

void HUDHealthBar::update(float deltatime)
{
}

void HUDHealthBar::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_freezBarSprite, states);
    target.draw(_healthBarSprite, states);
}

void HUDHealthBar::setup()
{
    setSize();
    setTextures();
    setPosition();
}

void HUDHealthBar::setSize()
{
    _freezBarSprite.setScale(0.3, 0.3);
    _healthBarSprite.setScale(0.3, 0.3);
}

void HUDHealthBar::setTextures()
{
    _healthBarSprite.setTexture(
        ResourseManager::getInstance()->getTextures(TextureType::Health_bar)[0]);
    _freezBarSprite.setTexture(
        ResourseManager::getInstance()->getTextures(TextureType::Health_bar)[0]);
}

void HUDHealthBar::setPosition()
{
    const sf::Vector2f viewSize = _gameView->getSize();
    const sf::Vector2f viewCenter = _gameView->getCenter();
    const auto healthTextureHeight = _healthBarSprite.getTexture()->getSize().y / 2;
    const sf::Vector2f bottomLeft(viewCenter.x - viewSize.x / 2.f, viewCenter.y + viewSize.y / 2.f);

    const sf::Vector2f freezBarPos{ bottomLeft.x, bottomLeft.y - healthTextureHeight };
    const sf::Vector2f healthBarPos{ bottomLeft.x, bottomLeft.y - (healthTextureHeight * 2) };

    _freezBarSprite.setPosition(freezBarPos);
    _healthBarSprite.setPosition(healthBarPos);
}
