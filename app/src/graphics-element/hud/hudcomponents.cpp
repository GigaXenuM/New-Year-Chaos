#include "hudcomponents.h"

HUDComponents::HUDComponents(sf::View *view)
    : _freezBar{ std::make_unique<HUDHealthBar>(view) },
      _healthBar{ std::make_unique<HUDHealthBar>(view) }
{
    _hudItems.push_back(_freezBar.get());
    _hudItems.push_back(_healthBar.get());
}

std::vector<Graphics::AbstractItem *> &HUDComponents::hudItems()
{
    return _hudItems;
}
