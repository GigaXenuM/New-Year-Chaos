#include "hudcomponents.h"

#include "player/player.h"
#include "resources/resourcemanager.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace Game
{

HUDComponents::HUDComponents(sf::RenderTarget *renderTarget, sf::View *view)
    : _gameView{ view },
      _teaIcon{ std::make_unique<TeaIcon>() },
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
    const std::vector<Graphics::AbstractItem *> hudItems{ _freezBar.get(), _healthBar.get(),
                                                          _teaIcon.get() };

    updateBarValue();
    updateBarPosition();
    for (const auto &hudItem : hudItems)
    {
        hudItem->update(deltatime);
        _renderTarget->draw(*hudItem);
    }
}

void HUDComponents::updateBarValue()
{
    _freezBar->setValue(gPlayer->getFreezPoints());
    _healthBar->setValue(gPlayer->getHealthPoints());
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
        { bottomLeft.x + 2,
          bottomLeft.y - (_freezBar->getSprite()->getGlobalBounds().height * 1.5f) });

    _teaIcon->setPosition({ _healthBar->getIconSprite()->getPosition().x
                                - _teaIcon->getSprite()->getGlobalBounds().width * 0.2f,
                            _healthBar->getIconSprite()->getPosition().y
                                - _teaIcon->getSprite()->getGlobalBounds().height });
}
} // namespace Game