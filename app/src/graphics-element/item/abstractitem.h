#pragma once

#include "drawable.h"
#include "event/eventhandler.h"

#include "geometry/point.h"
#include "geometry/rect.h"

enum class Align;

namespace Visitor
{
class IVisitor;
} // namespace Visitor

namespace Graphics
{

class AbstractItem : public Drawable
{
public:
    explicit AbstractItem(EventHandler *parent);
    virtual ~AbstractItem() = default;

    virtual RectF globalRect() const = 0;
    virtual RectF localRect() const = 0;
    virtual PointF center() const = 0;

    virtual void setPos(PointF position) = 0;
    virtual void setOrigin(Align origin) = 0;

    virtual void accept(Visitor::IVisitor *visitor);
};

} // namespace Graphics
