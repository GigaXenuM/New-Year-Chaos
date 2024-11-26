#include "abstractitem.h"

#include "visitor/ivisitor.h"

namespace Graphics
{
AbstractItem::AbstractItem(EventHandler *parent) : Drawable{ parent }
{
}

void AbstractItem::move(const sf::Vector2f delta)
{
    setPosition(position() + delta);
}

void AbstractItem::accept(Visitor::IVisitor *visitor)
{
    visitor->visitAbstractItem(this);
}
} // namespace Graphics
