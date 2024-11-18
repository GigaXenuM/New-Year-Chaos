#pragma once

#include "iupdatable.h"

#include <memory>

class EventHandler;

class Layout;

namespace sf
{
class RenderTarget;
} // namespace sf

namespace Scene
{
class AbstractItem;
} // namespace Scene

class Menu : public IUpdatable
{
public:
    explicit Menu(sf::RenderTarget *renderTarget, EventHandler *eventHandler);

    void update(float deltatime) override;

    void setLayout(std::unique_ptr<Layout> layout);

private:
    sf::RenderTarget *_renderTarget;
    std::unique_ptr<Layout> _layout;

    EventHandler *_eventHandler;
};
