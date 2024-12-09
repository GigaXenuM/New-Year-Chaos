#pragma once

#include "action/iaction.h"

#include <functional>

namespace Game
{

class PickUpTeaAction : public IAction
{
public:
    PickUpTeaAction(const sf::Vector2f &actionItemPos, std::function<void()> callback);

    ActionVariant actionVariant() const override;
    sf::Vector2f position() const override;
    void execute() override;

private:
    const sf::Vector2f _actionItemPos;
    std::function<void()> _callback;
};

} // namespace Game
