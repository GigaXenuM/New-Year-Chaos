#pragma once

#include "geometry/rect.h"
#include "layout.h"

#include "geometry/point.h"

class EventHandler;

class VerticalLayout : public Layout
{
public:
    explicit VerticalLayout(RectF rect);

protected:
    void updateGeometry() override;

private:
    SizeF contentSize() const;

    void alignCenter();
    void alignLeft();
    void alignRight();
    void alignTop();
    void alignBottom();

    PointF _nextItemPos{ 0.f, 0.f };
};
