#include "abstractitem.h"

#include "visitor/ivisitor.h"

namespace Graphics
{
AbstractItem::AbstractItem(EventHandler *parent) : Drawable{ parent }
{
}

void AbstractItem::accept(Visitor::IVisitor *visitor)
{
    visitor->visitAbstractItem(this);
}
} // namespace Graphics
