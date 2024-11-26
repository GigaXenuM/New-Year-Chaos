#pragma once

#include "iupdatable.h"

#include <memory>
#include <string>
#include <vector>

class Player;

namespace sf
{
class RenderTarget;
class Shape;
} // namespace sf

namespace Graphics
{
class Drawable;
} // namespace Graphics

namespace Scene::Level
{

class TileLayer;
class ObjectLayer;

class Controller : public IUpdatable
{
public:
    explicit Controller(sf::RenderTarget *renderTarget, EventHandler *parent);
    ~Controller();

    void update(float deltatime) override;

protected:
    void keyPressEvent(KeyPressEvent *event) override;
    void keyReleaseEvent(KeyReleaseEvent *event) override;

private:
    void loadLevel();
    void handleCollisions() const;

    sf::RenderTarget *_renderTarget;

    std::vector<std::unique_ptr<TileLayer>> _tileLayers;
    std::vector<std::unique_ptr<ObjectLayer>> _objectLayers;

    std::unique_ptr<Player> _player;
    std::vector<Graphics::Drawable *> _itemsToDrawing;
};

} // namespace Scene::Level
