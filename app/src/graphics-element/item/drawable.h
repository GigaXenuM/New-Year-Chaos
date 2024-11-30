#pragma once

#include "iupdatable.h"

#include "SFML/Graphics/Drawable.hpp"

namespace Graphics
{
class Drawable : public IUpdatable, public sf::Drawable
{
public:
    void update(float /*deltatime*/) override
    {
    }
};
} // namespace Graphics
