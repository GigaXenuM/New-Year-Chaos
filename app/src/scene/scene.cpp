#include "scene.h"

#include "item/abstractitem.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace Scene
{
Scene::Scene(sf::RenderTarget *renderTarget, EventHandler *parent)
    : EventHandler{ parent }, _renderTarget{ renderTarget }
{
}

void Scene::update(float deltatime)
{
    _renderTarget->clear(sf::Color(87, 179, 113, 255));

    for (const auto &item : _itemsToDrawing)
    {
        item->update(deltatime);
        _renderTarget->draw(*item);
    }
}

void Scene::addItem(Graphics::AbstractItem *item)
{
    _itemsToDrawing.push_back(item);
}
} // namespace Scene
