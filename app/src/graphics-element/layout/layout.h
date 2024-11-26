#pragma once

#include "geometry/alignment.h"
#include "util/enumflag.h"

#include <SFML/Graphics/Rect.hpp>

#include <memory>
#include <vector>

namespace Graphics
{
class ILayoutItem;
} // namespace Graphics

class Layout
{
    using Items = std::vector<std::shared_ptr<Graphics::ILayoutItem>>;

public:
    explicit Layout(const sf::FloatRect &rect);

    void addItem(std::shared_ptr<Graphics::ILayoutItem> item);
    Items items() const;

    void setAlignment(Align alingment);
    Util::EnumFlag<Align> alignment() const;

    const sf::FloatRect &rect() const;

    void setSpacing(float spacing);
    float spacing() const;

private:
    virtual void updateGeometry() = 0;

    Items _items;
    Util::EnumFlag<Align> _alignment{ Align::Left, Align::Right, Align::Top, Align::Bottom };
    sf::FloatRect _rect{};
    float _spacing{ 0.f };
};
