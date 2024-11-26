#pragma once

#include "eventhandler.h"

class IUpdatable : public EventHandler
{
public:
    IUpdatable(EventHandler *parent = nullptr) : EventHandler{ parent }
    {
    }

    virtual void update(float deltatime) = 0;
};
