#pragma once

#include "hud/hudhealthbar.h"
#include "item/abstractitem.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>

#include <vector>

class HUDComponents
{
public:
    HUDComponents(sf::View *view);

    void draw(sf::RenderTarget *renderTarget);

    [[nodiscard]] std::vector<Graphics::AbstractItem *> &hudItems();

private:
    std::vector<Graphics::AbstractItem *> _hudItems;

    std::unique_ptr<HUDHealthBar> _freezBar;
    std::unique_ptr<HUDHealthBar> _healthBar;
};
