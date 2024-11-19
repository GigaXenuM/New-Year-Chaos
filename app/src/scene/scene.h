#pragma once

#include "event/eventhandler.h"
#include "iupdatable.h"

#include <vector>

namespace sf
{
class RenderTarget;
} // namespace sf

namespace Graphics
{
class Drawable;
class AbstractItem;
} // namespace Graphics

namespace Scene
{

class Scene : public EventHandler, public IUpdatable
{
public:
    explicit Scene(sf::RenderTarget *renderTarget, EventHandler *parent);
    virtual ~Scene() = default;

    void update(float deltatime) override;

    void addItem(Graphics::AbstractItem *item);

private:
    sf::RenderTarget *_renderTarget{ nullptr };
    std::vector<Graphics::Drawable *> _itemsToDrawing;
};

} // namespace Scene
