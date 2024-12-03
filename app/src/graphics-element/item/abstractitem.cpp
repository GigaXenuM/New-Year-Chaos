#include "abstractitem.h"

#include "visitor/ivisitor.h"

namespace Graphics
{

void AbstractItem::accept(Visitor::IVisitor *visitor)
{
    visitor->visitAbstractItem(this);
}

} // namespace Graphics
