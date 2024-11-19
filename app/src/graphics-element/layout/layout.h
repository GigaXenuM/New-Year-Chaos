#pragma once

#include "geometry/alignment.h"
#include "geometry/rect.h"

#include <memory>
#include <vector>

namespace Graphics
{
class AbstractItem;
} // namespace Graphics

class Layout
{
    using Items = std::vector<std::shared_ptr<Graphics::AbstractItem>>;

public:
    explicit Layout(RectF rect);

    void addItem(std::shared_ptr<Graphics::AbstractItem> item);
    Items items() const;

    void setAlignment(Align alingment);
    Align alignment() const;

    RectF rect() const;

    void setSpacing(float spacing);
    float spacing() const;

private:
    virtual void updateGeometry() = 0;

    Items _items;
    Align _alignment{ Align::Center };
    RectF _rect{};
    float _spacing{ 0.f };
};
