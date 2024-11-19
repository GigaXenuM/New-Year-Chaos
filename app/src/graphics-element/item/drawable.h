#pragma once

#include "iupdatable.h"

#include "SFML/Graphics/Drawable.hpp"

namespace Graphics
{
class Drawable : public IUpdatable, public sf::Drawable
{
public:
    Drawable(EventHandler *parent) : IUpdatable{ parent }
    {
    }

    void update(float /*deltatime*/) override
    {
    }
};
} // namespace Graphics
