#pragma once

#include "iupdatable.h"

namespace sf
{
class View;
}

class IView : public IUpdatable
{
public:
    IView(EventHandler *parent) : IUpdatable{ parent }
    {
    }

    virtual sf::View *view() const = 0;
};
