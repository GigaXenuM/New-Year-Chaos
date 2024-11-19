#pragma once

#include <functional>

class IButton
{
protected:
    using OnClickCallback = std::function<void()>;

public:
    virtual void onClick(OnClickCallback callback) = 0;
};
