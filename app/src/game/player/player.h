#pragma once

#include "action/iactionhandler.h"
#include "animation/animation.h"
#include "items/entity/physicalentity.h"
#include "tips/hint.h"
#include "util/limitedvalue.h"

#include <SFML/Graphics/Sprite.hpp>

#include <box2d/b2_body.h>

#include <set>
#include <unordered_map>

struct b2Body;

namespace Game
{

class Player final : public PhysicalEntity, public IActionHandler
{
public:
    Player(b2World *world, sf::Shape *shape, bool menuMode = false);

    void restoreFreezePoints();
    void restoreHealthPoints();
    void addTeaItem();
    void addHealItem();

    [[nodiscard]] bool isWon() const;
    [[nodiscard]] bool isDead() const;
    [[nodiscard]] float getFreezPoints() const;
    [[nodiscard]] float getHealthPoints() const;
    [[nodiscard]] float getStaminaPoints() const;
    [[nodiscard]] size_t getTeaCount() const;
    [[nodiscard]] size_t getAidKitCount() const;

    [[nodiscard]] sf::Sprite &getSprite();
    [[nodiscard]] sf::Vector2f getPosition() const;

    const Drawable *hint() const;

    void damage(float power) override;
    void freezeDamage(float power);

    void kill();
    void setWinStatus(const bool status);

    void visitActions(const std::vector<IAction *> &actions) override;
    void executeAvailableAction() override;

    void setMentadoryHint(std::string hintText, bool disposable);

protected:
    void update(float deltatime) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    void updatePosition(float deltatime);
    void updateAnimation(float deltatime);
    void updateHealthPoint(float deltatime);
    void updateStaminaPoint(float deltatime);
    void updateHint(float deltatime);

    void restoreStaminaPoints(float deltatime);

    std::string hintText(IAction *action);

private:
    const bool _menuMode{ false };

    const float _scale{ 0.25f };
    const float _healthUpdateInterval{ 1.0f };

    Util::LimitedValueF _freeze{ 100.f, 0.f, 100.f };
    Util::LimitedValueF _health{ 100.f, 0.f, 100.f };

    float _healthUpdateTimer{ 0.0f };

    bool _isWon{ false };
    bool _hasKey{ false };

    sf::Sprite _sprite;

    size_t _countOfTeaItem{ 2 };
    size_t _countOfHealthItem{ 1 };

    Animation _runAnimation;
    Animation _deadAnimation;
    Animation _walkAnimation;
    Animation _idleAnimation;
    Animation _jumpAnimation;

    IAction *_availableAction{ nullptr };

    Hint _hint;

    std::unordered_map<std::string, bool> _mandatoryHints;
    std::set<std::string> _showedMandatoryHints;
    Util::LimitedValueF _mandatoryHintTimer;
};

} // namespace Game
