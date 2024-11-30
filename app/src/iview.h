#pragma once

#include "eventhandler.h"
#include "iupdatable.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace sf
{
class View;
}

class IView : public IUpdatable, public EventHandler
{
public:
    IView(sf::RenderTarget *renderTarget, EventHandler *parent)
        : EventHandler{ parent }, _renderTarget{ renderTarget }
    {
    }

    void update(float deltatime)
    {
        _renderTarget->setView(*view());
    }

    virtual sf::View *view() const = 0;

private:
    sf::RenderTarget *_renderTarget{ nullptr };
};
