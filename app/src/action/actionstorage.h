#pragma once

#include <cstdlib>
#include <functional>
#include <unordered_map>

namespace Action
{

class ActionStorage
{
public:
    virtual ~ActionStorage() = default;

    template <typename T> void registerAction(T key, std::function<void()> action)
    {
        static_assert(std::is_enum_v<T>,
                      "ActionStorage::registerAction: T must be enumeration type.");

        const auto castedKey{ static_cast<size_t>(key) };
        _actions[castedKey].push_back(std::move(action));
    }

    template <typename T> void executeActions(T key)
    {
        static_assert(std::is_enum_v<T>,
                      "ActionStorage::executeActions: T must be enumeration type.");

        const auto castedKey{ static_cast<size_t>(key) };
        for (const auto &action : _actions[castedKey])
            action();
    }

private:
    std::unordered_map<size_t, std::vector<std::function<void()>>> _actions;
};

} // namespace Action
