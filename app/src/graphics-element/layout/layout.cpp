#include "layout.h"

#include "item/ilayoutitem.h"

Layout::Layout(const sf::FloatRect &rect) : _rect{ rect }
{
}

void Layout::addItem(std::shared_ptr<Graphics::ILayoutItem> item)
{
    if (std::find(_items.cbegin(), _items.cend(), item) != _items.cend())
        return;

    _items.push_back(std::move(item));

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

Util::EnumFlag<Align> Layout::alignment() const
{
    return _alignment;
}

void Layout::setRect(const sf::FloatRect &rect)
{
    _rect = rect;
    updateGeometry();
}

const sf::FloatRect &Layout::rect() const
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
