#include "menuview.h"

#include "geometry/utils.h"

#include "SFML/Graphics/RenderTarget.hpp"

MenuView::MenuView(sf::RenderTarget *renderTarget, RectF rect, EventHandler *parent)
    : sf::View{ Geometry::toSfmlRect(rect) }, _renderTarget{ renderTarget }
{
    if (parent != nullptr)
        parent->addEventHandler(this);
}

void MenuView::update(float /*deltaTime*/)
{
    _renderTarget->setView(*this);
}
