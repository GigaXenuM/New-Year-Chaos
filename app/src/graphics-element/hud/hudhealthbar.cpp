#include "hudhealthbar.h"

#include "resources/resourcemanager.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <cassert>

HUDHealthBar::HUDHealthBar(const sf::Texture &iconTexture)
{
    _barIcon.setTexture(iconTexture);
    setup();
}

const sf::Sprite *HUDHealthBar::getSprite() const
{
    return &_healthBar;
}

void HUDHealthBar::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_healthBar, states);
    target.draw(_health, states);
    target.draw(_barIcon, states);
}

void HUDHealthBar::setup()
{
    setTextures();
    setSize();
}

void HUDHealthBar::setSize()
{
    _healthBar.setScale(0.3, 0.3);
    _barIcon.setScale(_iconScaleFactors.first, _iconScaleFactors.second);

    const sf::Vector2f healthValueRect
        = { _healthBar.getGlobalBounds().width - 12, _healthBar.getGlobalBounds().height / 2.2f };
    _health.setSize(healthValueRect);
}

void HUDHealthBar::setTextures()
{
    _healthBar.setTexture(ResourseManager::getInstance()->getTextures(TextureType::Health_bar)[0]);
}

void HUDHealthBar::setPosition(const sf::Vector2f pos)
{
    const sf::Vector2f texturePos = { pos.x + _barIcon.getGlobalBounds().width,
                                      pos.y - _healthBar.getGlobalBounds().height * 3 };
    _healthBar.setPosition(texturePos);

    _health.setPosition(texturePos.x + 6,
                        texturePos.y + (_healthBar.getGlobalBounds().height * 0.28));

    _barIcon.setPosition(texturePos.x - _barIcon.getGlobalBounds().width, texturePos.y);
}

void HUDHealthBar::setIconScaleFactors(float factorX, float factorY)
{
    _barIcon.setScale(factorX, factorY);
}

void HUDHealthBar::setColor(const sf::Color &color)
{
    _health.setFillColor(color);
}

void HUDHealthBar::setValue(const float value)
{
    if (value > 100 || value < 0)
        assert(false && "HUDHealthBar::setValue the value cannot exceed 100 and be less than 0");
    sf::Vector2f currentSize = _health.getSize();
    currentSize.x = (_healthBar.getGlobalBounds().width - 12) * (value / 100.f);
    _health.setSize(currentSize);
}
