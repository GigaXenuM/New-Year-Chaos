#pragma once

#include "drawable.h"
#include "event/eventhandler.h"

#include "geometry/point.h"
#include "geometry/rect.h"

enum class Align;

namespace Scene
{
class IVisitor;

class AbstractItem : public EventHandler, public Drawable
{
public:
    explicit AbstractItem(EventHandler *parent);
    virtual ~AbstractItem() = default;

    virtual RectF globalRect() const = 0;
    virtual RectF localRect() const = 0;
    virtual PointF center() const = 0;

    virtual void setPos(PointF position) = 0;
    virtual void setOrigin(Align origin) = 0;

    virtual void accept(IVisitor *visitor);
};
} // namespace Scene
