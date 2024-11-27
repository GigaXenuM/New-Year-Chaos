#pragma once

#include "hud/hudhealthbar.h"
#include "item/abstractitem.h"

#include <memory>
#include <vector>

namespace sf
{
class View;
}

class HUDComponents
{
public:
    HUDComponents(sf::RenderTarget *renderTarget, sf::View *view);

    void update(const float deltatime);

private:
    void updateBarPosition();

private:
    sf::View *_gameView{ nullptr };
    sf::RenderTarget *_renderTarget{ nullptr };

    std::unique_ptr<HUDHealthBar> _freezBar;
    std::unique_ptr<HUDHealthBar> _healthBar;
};
