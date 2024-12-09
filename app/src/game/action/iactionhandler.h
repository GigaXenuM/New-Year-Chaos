#pragma once

#include <vector>

namespace Game
{

class IAction;

class IActionHandler
{
public:
    virtual ~IActionHandler() = default;

    virtual void visitActions(const std::vector<IAction *> &actions) = 0;
    virtual void executeAvailableAction() = 0;
};

} // namespace Game
