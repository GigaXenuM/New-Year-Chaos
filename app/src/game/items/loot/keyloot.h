#pragma once

#include "action/iaction.h"
#include "items/abstractphysicalitem.h"
#include "tips/hint.h"

#include <SFML/Graphics/Sprite.hpp>
#include <functional>

namespace Game
{

class KeyLoot : public AbstractPhysicalItem, public IAction
{
public:
    KeyLoot(b2World *world, sf::Shape *shape);

    ItemType type() const override
    {
        return ItemType::Loot;
    }
    ActionVariant actionVariant() const override
    {
        return ActionVariant::PickUpTea;
    }

    void showHint();
    void hideHint();

    bool needDestroying() const override;
    void prepareDestroy();

    void setCallback(std::function<void()> actionCallback);

    void update(float deltatime) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    sf::Vector2f position() const override;
    void execute() override;

private:
    sf::Sprite _sprite;
    Hint _hint;
    std::function<void()> _actionCallback;
    float _needDestroy{ false };

    bool _isNeedShowHint{ false };

private:
    float _fadeTime = 0.0f;
    float _fadeDuration = 3.0f;
};

} // namespace Game
