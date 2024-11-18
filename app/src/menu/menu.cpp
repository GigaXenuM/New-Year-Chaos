#include "menu.h"

#include "layout/verticallayout.h"
#include "scene/items/abstractitem.h"

#include "SFML/Graphics/RenderTarget.hpp"

Menu::Menu(sf::RenderTarget *renderTarget, EventHandler *eventHandler)
    : _renderTarget{ renderTarget }
{
}

void Menu::update(float /*deltatime*/)
{
    using Item = std::shared_ptr<Scene::AbstractItem>;

    _renderTarget->clear(sf::Color(50, 56, 59, 255));

    for (const Item &item : _layout->items())
        _renderTarget->draw(*item);
}

void Menu::setLayout(std::unique_ptr<Layout> layout)
{
    _layout = std::move(layout);
}
