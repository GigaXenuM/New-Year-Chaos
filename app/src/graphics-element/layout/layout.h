#pragma once

#include "eventhandler.h"
#include "geometry/alignment.h"
#include "util/enumflag.h"
#include "util/geometryoperation.h"

#include <SFML/Graphics/Rect.hpp>

#include <memory>
#include <vector>

namespace Graphics
{
class ILayoutItem;
} // namespace Graphics

class Layout : public EventHandler
{
    using Items = std::vector<std::shared_ptr<Graphics::ILayoutItem>>;

public:
    explicit Layout(EventHandler *parent, const sf::FloatRect &rect = {});
    virtual ~Layout() = default;

    void addItem(std::shared_ptr<Graphics::ILayoutItem> item);
    Items items() const;

    void setAlignment(Align alingment);
    Util::EnumFlag<Align> alignment() const;

    void setRect(const sf::FloatRect &rect);
    const sf::FloatRect &rect() const;

    void setSpacing(float spacing);
    float spacing() const;

private:
    virtual void updateGeometry() = 0;

    Items _items;
    Util::EnumFlag<Align> _alignment{ Util::ALIGN_CENTER_STATE };
    sf::FloatRect _rect{};
    float _spacing{ 0.f };
};
