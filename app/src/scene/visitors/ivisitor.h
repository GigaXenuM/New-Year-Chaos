#pragma once

namespace Scene
{

class AbstractItem;

class IVisitor
{
public:
    ~IVisitor() = default;

    virtual void visitSceneItem(AbstractItem *item) = 0;
};

} // namespace Scene
