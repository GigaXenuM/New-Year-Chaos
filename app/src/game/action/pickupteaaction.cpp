#include "pickupteaaction.h"

namespace Game
{

PickUpTeaAction::PickUpTeaAction(const sf::Vector2f &actionItemPos, std::function<void()> callback)
    : _actionItemPos{ actionItemPos }, _callback{ std::move(callback) }
{
}

ActionVariant PickUpTeaAction::actionVariant() const
{
    return ActionVariant::PickUpTea;
}

sf::Vector2f PickUpTeaAction::position() const
{
    return _actionItemPos;
}

void PickUpTeaAction::execute()
{
    _callback();
}

} // namespace Game
