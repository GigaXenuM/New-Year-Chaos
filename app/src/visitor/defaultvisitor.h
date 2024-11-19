#pragma once

#include "visitor/ivisitor.h"

namespace Visitor
{

class DefaultVisitor : public IVisitor
{
public:
    DefaultVisitor() = default;

    virtual void visitAbstractItem(Graphics::AbstractItem *item) override;
};
} // namespace Visitor
