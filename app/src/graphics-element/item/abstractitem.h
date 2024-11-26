#pragma once

#include "drawable.h"
#include "event/eventhandler.h"

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
    explicit AbstractItem(EventHandler *parent);
    virtual ~AbstractItem() = default;

    virtual void setPosition(const sf::Vector2f &position) = 0;
    virtual sf::Vector2f position() const = 0;

    void move(const sf::Vector2f delta);

    virtual sf::FloatRect globalRect() const = 0;
    virtual sf::FloatRect localRect() const = 0;

    virtual void accept(Visitor::IVisitor *visitor);
};

} // namespace Graphics
