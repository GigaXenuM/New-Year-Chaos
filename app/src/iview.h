#pragma once

#include "iupdatable.h"

namespace sf
{
class View;
}

class IView : public IUpdatable
{
public:
    virtual sf::View *view() const = 0;
};
