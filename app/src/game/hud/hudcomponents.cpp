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
          ResourseManager::getInstance()->getTextures(TextureType::HeartBar_icon)[0]) },
      _staminaBar{ std::make_unique<HUDHealthBar>(
          ResourseManager::getInstance()->getTextures(TextureType::Stamina_icon)[0]) },
      _aidKitIcon{ std::make_unique<TeaIcon>() }
{
    _aidKitIcon->setTexture(
        ResourseManager::getInstance()->getTextures(TextureType::Aid_Kit_loot).front());
    _teaIcon->setTexture(ResourseManager::getInstance()->getTextures(TextureType::Tea).front());
    _aidKitIcon->setScaleFactor(0.09, 0.09);
    _freezBar->setColor(sf::Color::Blue);
    _healthBar->setColor(sf::Color::Red);
    _staminaBar->setColor(sf::Color::Green);
}

void HUDComponents::update(const float deltatime)
{
    const std::vector<Graphics::AbstractItem *> hudItems{ _freezBar.get(),   _healthBar.get(),
                                                          _teaIcon.get(),    _weaponIcon.get(),
                                                          _staminaBar.get(), _aidKitIcon.get() };

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
    _teaIcon->updateHealCount(_player->getTeaCount());
    _freezBar->setValue(_player->getFreezPoints());
    _healthBar->setValue(_player->getHealthPoints());
    _staminaBar->setValue(_player->getStaminaPoints());
    _aidKitIcon->updateHealCount(_player->getAidKitCount());
}

void HUDComponents::updateBarPosition()
{
    constexpr float yDistBetweenItems{ 30 };
    const sf::Vector2f viewSize = _gameView->getSize();
    const sf::Vector2f viewCenter = _gameView->getCenter();
    const sf::Vector2f bottom(viewCenter.x, viewCenter.y + viewSize.y / 2.f);
    const sf::Vector2f bottomLeft(viewCenter.x - viewSize.x / 2.f, viewCenter.y + viewSize.y / 2.f);

    _freezBar->setPosition(bottomLeft);
    _freezBar->setIconScaleFactors(0.07, 0.07);

    _healthBar->setPosition(
        { bottomLeft.x + 2,
          bottomLeft.y - (_freezBar->getSprite()->getGlobalBounds().height * 1.5f) });

    _aidKitIcon->setPosition({ bottom.x + _aidKitIcon->globalRect().width * 1.5f,
                               bottom.y - _aidKitIcon->getSprite()->getGlobalBounds().height });

    _teaIcon->setPosition({ bottom.x, bottom.y - _teaIcon->getSprite()->getGlobalBounds().height });

    _weaponIcon->setPosition({ bottom.x - _weaponIcon->globalRect().width * 1.5f,
                               bottom.y - _weaponIcon->getSprite()->getGlobalBounds().height + 3 });

    _staminaBar->setPosition(
        { bottomLeft.x, bottomLeft.y + _staminaBar->getSprite()->getGlobalBounds().height
                            + yDistBetweenItems / 2 });
    _staminaBar->setIconScaleFactors(0.08, 0.08);
}
} // namespace Game
