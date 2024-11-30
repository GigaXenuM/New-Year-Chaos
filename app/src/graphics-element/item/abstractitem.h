#pragma once

#include "drawable.h"

#include <SFML/System/Vector2.hpp>

enum class Align;

namespace Visitor
{
class IVisitor;
} // namespace Visitor

namespace Graphics
{

class AbstractItem : public Drawable
{
public:
    virtual ~AbstractItem() = default;

    virtual void setPosition(const sf::Vector2f &position)
    {
    }
    virtual sf::Vector2f position() const
    {
        return {};
    }

    void move(const sf::Vector2f delta);

    virtual sf::FloatRect globalRect() const
    {
        return {};
    }
    virtual sf::FloatRect localRect() const
    {
        return {};
    }

    virtual void accept(Visitor::IVisitor *visitor);
};

} // namespace Graphics
