#include "layout.h"

#include "geometry/rect.h"
#include "scene/items/abstractitem.h"

Layout::Layout(RectF rect, EventHandler *eventHandler) : _rect{ rect }
{
}

void Layout::addItem(std::shared_ptr<Scene::AbstractItem> item)
{
    if (std::find(_items.cbegin(), _items.cend(), item) != _items.cend())
        return;

    _items.push_back(item);

    updateGeometry();
}

Layout::Items Layout::items() const
{
    return _items;
}

void Layout::setAlignment(Align alingment)
{
    _alignment = alingment;
    updateGeometry();
}

Align Layout::alignment() const
{
    return _alignment;
}

RectF Layout::rect() const
{
    return _rect;
}

void Layout::setSpacing(float spacing)
{
    _spacing = spacing;
    updateGeometry();
}

float Layout::spacing() const
{
    return _spacing;
}
