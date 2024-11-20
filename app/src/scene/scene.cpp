#include "scene.h"

#include "item/abstractitem.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace Scene
{
Scene::Scene(sf::RenderTarget *renderTarget, EventHandler *parent)
    : IView{ parent },
      _renderTarget{ renderTarget },
      _view{ std::make_unique<sf::View>(
          sf::FloatRect(sf::Vector2f{},
                        sf::Vector2f(renderTarget->getSize().x, renderTarget->getSize().y))) },
      _player{ std::make_unique<Player>(this) }
{
    addItem(_player.get());
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

sf::View *Scene::view() const
{
    return _view.get();
}

void Scene::addItem(Graphics::AbstractItem *item)
{
    _itemsToDrawing.push_back(item);
}
} // namespace Scene
