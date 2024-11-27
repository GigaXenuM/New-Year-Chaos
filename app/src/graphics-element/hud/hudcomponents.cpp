#include "hudcomponents.h"

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "resources/resourceManager.h"

HUDComponents::HUDComponents(sf::RenderTarget *renderTarget, sf::View *view)
    : _gameView{ view },
      _renderTarget{ renderTarget },
      _freezBar{ std::make_unique<HUDHealthBar>(
          ResourseManager::getInstance()->getTextures(TextureType::FreezBar_icon)[0]) },
      _healthBar{ std::make_unique<HUDHealthBar>(
          ResourseManager::getInstance()->getTextures(TextureType::HeartBar_icon)[0]) }
{
    _freezBar->setColor(sf::Color::Blue);
    _healthBar->setColor(sf::Color::Red);
}

void HUDComponents::update(const float deltatime)
{
    const std::vector<Graphics::AbstractItem *> hudItems{ _freezBar.get(), _healthBar.get() };

    updateBarPosition();
    for (const auto &hudItem : hudItems)
    {
        hudItem->update(deltatime);
        _renderTarget->draw(*hudItem);
    }
}

void HUDComponents::updateBarPosition()
{
    constexpr float yDistBetweenItems{ 30 };
    const sf::Vector2f viewSize = _gameView->getSize();
    const sf::Vector2f viewCenter = _gameView->getCenter();
    const sf::Vector2f bottomLeft(viewCenter.x - viewSize.x / 2.f, viewCenter.y + viewSize.y / 2.f);

    _freezBar->setPosition(bottomLeft);
    _freezBar->setIconScaleFactors(0.07, 0.07);
    _healthBar->setPosition(
        { bottomLeft.x + 2, bottomLeft.y - (_freezBar->getSprite()->getGlobalBounds().height * 1.5f) });
}
