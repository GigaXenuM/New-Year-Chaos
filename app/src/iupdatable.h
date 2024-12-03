#pragma once

class IUpdatable
{
public:
    virtual void update(float deltatime) = 0;
};
