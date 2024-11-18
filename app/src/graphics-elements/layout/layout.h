#pragma once

#include "geometry/alignment.h"
#include "geometry/rect.h"

#include <memory>
#include <vector>

class EventHandler;

namespace Scene
{
class AbstractItem;
} // namespace Scene

class Layout
{
    using Items = std::vector<std::shared_ptr<Scene::AbstractItem>>;

public:
    explicit Layout(RectF rect, EventHandler *eventHandler);

    void addItem(std::shared_ptr<Scene::AbstractItem> item);
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
