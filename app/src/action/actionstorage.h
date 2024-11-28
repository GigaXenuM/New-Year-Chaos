#pragma once

#include <cstdlib>
#include <functional>
#include <unordered_map>

namespace Action
{

template <typename T> class ActionStorage
{
    static_assert(std::is_enum_v<T>, "ActionStorage::registerAction: T must be enumeration type.");

public:
    virtual ~ActionStorage() = default;

    void registerAction(T key, std::function<void()> action)
    {

        const auto castedKey{ static_cast<size_t>(key) };
        _actions[castedKey].push_back(std::move(action));
    }

    void executeActions(T key)
    {
        const auto castedKey{ static_cast<size_t>(key) };
        for (const auto &action : _actions[castedKey])
            action();
    }

private:
    std::unordered_map<size_t, std::vector<std::function<void()>>> _actions;
};

} // namespace Action
