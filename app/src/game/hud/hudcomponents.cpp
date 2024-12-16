#include "hudcomponents.h"

#include "player/player.h"
#include "resources/resourcemanager.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace Game
{

HUDComponents::HUDComponents(sf::RenderTarget *renderTarget, sf::View *view, const Player *player)
    : _renderTarget{ renderTarget },
      _gameView{ view },
      _player{ player },
      _teaIcon{ std::make_unique<TeaIcon>() },
      _weaponIcon{ std::make_shared<WeaponIcon>(_player, sf::Vector2f{ 50.f, 50.f }) },
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
                                                          _teaIcon.get(), _weaponIcon.get() };

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
    _teaIcon->updateHealCount(_player->getHealCount());
    _freezBar->setValue(_player->getFreezPoints());
    _healthBar->setValue(_player->getHealthPoints());
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

    _teaIcon->setPosition(
        { Util::pointBy(_healthBar->getIconSprite()->getGlobalBounds(), Util::ALIGN_CENTER_STATE).x,
          Util::pointBy(_healthBar->getIconSprite()->getGlobalBounds(), Align::Top).y
              - (_teaIcon->globalRect().height / 2) });

    _weaponIcon->setPosition(
        { Util::pointBy(_teaIcon->getSprite()->getGlobalBounds(), Util::ALIGN_CENTER_STATE).x,
          Util::pointBy(_teaIcon->getSprite()->getGlobalBounds(), Align::Top).y
              - (_weaponIcon->globalRect().height / 2) });
}
} // namespace Game
