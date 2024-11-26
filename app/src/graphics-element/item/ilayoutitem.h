#pragma once

#include "item/abstractitem.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

namespace Util
{
template <typename T> class EnumFlag;
} // namespace Util

namespace Graphics
{

class ILayoutItem : public AbstractItem
{
public:
    ILayoutItem(EventHandler *parent) : AbstractItem{ parent }
    {
    }

    virtual sf::Vector2f center() const = 0;

    virtual void setOrigin(const Util::EnumFlag<Align> &origin) = 0;
};

} // namespace Graphics
