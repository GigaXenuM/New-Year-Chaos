#pragma once

#include "eventhandler.h"
#include "item/abstractitem.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

namespace Util
{
template <typename T> class EnumFlag;
} // namespace Util

namespace Graphics
{

class ILayoutItem : public AbstractItem, public EventHandler
{
public:
    ILayoutItem(EventHandler *parent) : EventHandler{ parent }
    {
    }

    virtual void setOrigin(const Util::EnumFlag<Align> &origin) = 0;
};

} // namespace Graphics
