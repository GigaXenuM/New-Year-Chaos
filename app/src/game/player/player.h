#pragma once

#include "action/iactionhandler.h"
#include "animation/animation.h"
#include "items/entity/physicalentity.h"
#include "tips/hint.h"

#include <SFML/Graphics/Sprite.hpp>

#include <box2d/b2_body.h>

struct b2Body;

namespace Game
{

class Player : public PhysicalEntity, public IActionHandler
{
public:
    Player(b2World *world, sf::Shape *shape);

    void health();
    void addHealthItem();

    [[nodiscard]] bool isDead() const;
    [[nodiscard]] float getFreezPoints() const;
    [[nodiscard]] float getHealthPoints() const;
    [[nodiscard]] float getStaminaPoints() const;
    [[nodiscard]] size_t getHealCount() const;

    [[nodiscard]] sf::Sprite &getSprite();
    [[nodiscard]] sf::Vector2f getPosition() const;

    const Drawable *hint() const;

    void damage(float power) override;

    void kill();

    void visitActions(const std::vector<IAction *> &actions) override;
    void executeAvailableAction() override;

protected:
    void update(float deltatime) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    void updatePosition(float deltatime);
    void updateAnimation(float deltatime);
    void updateHealthPoint(float deltatime);
    void updateStaminaPoint(float deltatime);
    void updateHint(float deltatime);

    void restoreHealthAndFreezePoints();
    void restoreStaminaPoints(float deltatime);

    std::string hintText(IAction *action) const;

private:
    const float _scale{ 0.25f };
    const float _healthUpdateInterval{ 1.0f };

    float _freeze{ 100.f };
    float _health{ 100.f };

    float _healthUpdateTimer{ 0.0f };

    bool _isHealthNeeded{ false };
    bool _hasKey{ false };

    sf::Sprite _sprite;

    size_t _countOfHealthItem{ 2 };

    Animation _runAnimation;
    Animation _deadAnimation;
    Animation _walkAnimation;
    Animation _idleAnimation;
    Animation _jumpAnimation;

    IAction *_availableAction{ nullptr };

    Hint _hint;
};

} // namespace Game
