#pragma once

#include "item/abstractitem.h"

#include "util/enumflag.h"

#include <SFML/Graphics/RectangleShape.hpp>

namespace Graphics
{

class PhisicalItem : public AbstractItem
{
public:
    enum class State
    {
        OnGround,
        Left,
        Right,
    };

    struct PhisicalContext
    {
        const float gravity{ 0.0f };
        const float speed{ 0.0f };
        const float limitVerticalSpeed{ 0.0f };
    };

    PhisicalItem(sf::RectangleShape *collider, const PhisicalContext &context,
                 EventHandler *parent);

    void setPosition(const sf::Vector2f &position) override;
    sf::Vector2f position() const override;

    sf::FloatRect globalRect() const override;
    sf::FloatRect localRect() const override;

    void handleCollision(const sf::Shape &externalCollider);
    void updateState(State state, bool isActive);
    bool isStateActive(State state) const;

protected:
    void update(float deltatime) override;

private:
    void updatePosition(float deltatime);
    sf::Vector2f gravityCorrection();
    void handleMoving(const sf::Vector2f &delta, float deltatime);

    sf::RectangleShape *_collider{ nullptr };
    PhisicalContext _context;
    float _verticalVelocity{ 0.f };
    sf::Vector2f _collisionCorrection;
    Util::EnumFlag<State> _state;
};

} // namespace Graphics
