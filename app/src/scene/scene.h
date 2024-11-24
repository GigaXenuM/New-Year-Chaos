#pragma once

#include "event/eventhandler.h"
#include "iview.h"
#include "player/player.h"

#include <memory>
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

class Scene : public IView
{
public:
    explicit Scene(sf::RenderTarget *renderTarget, EventHandler *parent);
    virtual ~Scene() = default;

    void update(float deltatime) override;
    sf::View *view() const override;

    void addItem(Graphics::AbstractItem *item);

private:
    sf::RenderTarget *_renderTarget{ nullptr };
    std::unique_ptr<sf::View> _view;

    std::unique_ptr<Player> _player;

    std::vector<Graphics::Drawable *> _itemsToDrawing;
};

} // namespace Scene
