#include "abstractitem.h"

#include "visitor/ivisitor.h"

namespace Graphics
{
AbstractItem::AbstractItem(EventHandler *parent) : EventHandler{ parent }
{
}

void AbstractItem::accept(Visitor::IVisitor *visitor)
{
    visitor->visitAbstractItem(this);
}
} // namespace Graphics
