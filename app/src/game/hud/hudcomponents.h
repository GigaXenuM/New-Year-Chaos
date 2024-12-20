#pragma once

#include "hudhealthbar.h"

#include "teaicon.h"
#include "weaponicon.h"

#include <memory>

namespace sf
{
class View;
}

namespace Game
{

class Player;

class HUDComponents : public IUpdatable
{
public:
    HUDComponents(sf::RenderTarget *renderTarget, sf::View *view, const Player *player);

    void update(float deltatime) override;

private:
    void updateBarValue();
    void updateBarPosition();

private:
    sf::RenderTarget *_renderTarget{ nullptr };
    sf::View *_gameView{ nullptr };
    const Player *_player{ nullptr };

    std::unique_ptr<TeaIcon> _teaIcon;
    std::unique_ptr<TeaIcon> _aidKitIcon;
    std::shared_ptr<WeaponIcon> _weaponIcon;
    std::unique_ptr<HUDHealthBar> _freezBar;
    std::unique_ptr<HUDHealthBar> _healthBar;
    std::unique_ptr<HUDHealthBar> _staminaBar;
};

} // namespace Game
