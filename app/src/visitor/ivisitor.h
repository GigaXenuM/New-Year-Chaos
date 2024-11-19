#pragma once

namespace Graphics
{
class AbstractItem;
} // namespace Graphics

namespace Visitor
{

class IVisitor
{
public:
    virtual ~IVisitor() = default;

    virtual void visitAbstractItem(Graphics::AbstractItem *item) = 0;
};

} // namespace Visitor
