#pragma once

#include "visitors/ivisitor.h"

namespace Scene
{

class DefaultVisitor : public IVisitor
{
public:
    DefaultVisitor() = default;

    virtual void visitSceneItem(AbstractItem *item) override;
};
} // namespace Scene
