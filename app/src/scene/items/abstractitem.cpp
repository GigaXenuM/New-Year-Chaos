#include "abstractitem.h"

#include "visitors/ivisitor.h"

namespace Scene
{
AbstractItem::AbstractItem(EventHandler *parent)
{
    if (parent != nullptr)
        parent->addEventHandler(this);
}

void AbstractItem::accept(IVisitor *visitor)
{
    visitor->visitSceneItem(this);
}
} // namespace Scene
