#pragma once

#include "hud/hudhealthbar.h"

#include <memory>

namespace sf
{
class View;
}

class HUDComponents : public IUpdatable
{
public:
    HUDComponents(sf::RenderTarget *renderTarget, sf::View *view);

    void update(float deltatime) override;

private:
    void updateBarValue();
    void updateBarPosition();

private:
    sf::View *_gameView{ nullptr };
    sf::RenderTarget *_renderTarget{ nullptr };

    std::unique_ptr<HUDHealthBar> _freezBar;
    std::unique_ptr<HUDHealthBar> _healthBar;
};
