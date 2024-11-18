#pragma once

class IView
{
public:
    virtual void update(float deltaTime) = 0;
};
