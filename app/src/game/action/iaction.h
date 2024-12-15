#pragma once

#include "actionvariant.h"

#include <SFML/System/Vector2.hpp>

#include <string>

namespace Game
{

class IAction
{
public:
    virtual ~IAction() = default;

    virtual ActionVariant actionVariant() const = 0;
    virtual sf::Vector2f position() const = 0;
    virtual void execute() = 0;

    virtual std::string hintText() const
    {
        return {};
    }
};

} // namespace Game
