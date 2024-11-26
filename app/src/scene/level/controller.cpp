#include "controller.h"

#include "keyevents/keypressevent.h"
#include "keyevents/keyreleaseevent.h"

#include "object-layer/objectlayer.h"
#include "player/player.h"
#include "tile-layer/tilelayer.h"

#include <tmxlite/Layer.hpp>
#include <tmxlite/Map.hpp>

namespace Scene::Level
{

Controller::Controller(sf::RenderTarget *renderTarget, EventHandler *parent)
    : IUpdatable{ parent }, _renderTarget{ renderTarget }
{
    loadLevel();
    _player = std::make_unique<Player>(dynamic_cast<sf::RectangleShape *>(
                                           _objectLayers.front()->getObjects("player").front()),
                                       nullptr);
    _itemsToDrawing.push_back(_player.get());
}

Controller::~Controller() = default;

void Controller::update(float deltatime)
{
    for (const auto &layer : _tileLayers)
    {
        layer->update(sf::seconds(deltatime));
        _renderTarget->draw(*layer);
    }

    for (const auto &layer : _objectLayers)
        _renderTarget->draw(*layer);

    for (const auto &item : _itemsToDrawing)
    {
        item->update(deltatime);
        _renderTarget->draw(*item);
    }

    handleCollisions();
}

void Controller::keyPressEvent(KeyPressEvent *event)
{
    if (event->key() == sf::Keyboard::D)
        _player->updateState(Player::State::Right, true);
    if (event->key() == sf::Keyboard::A)
        _player->updateState(Player::State::Left, true);
}

void Controller::keyReleaseEvent(KeyReleaseEvent *event)
{
    if (event->key() == sf::Keyboard::D)
        _player->updateState(Player::State::Right, false);
    if (event->key() == sf::Keyboard::A)
        _player->updateState(Player::State::Left, false);
}

void Controller::loadLevel()
{
    tmx::Map map;
    map.load("level/terrain.tmx");
    if (map.getOrientation() != tmx::Orientation::Orthogonal)
    {
        std::cerr << "Map is not orthogonal - nothing will be drawn" << std::endl;
        return;
    }

    const auto &layers{ map.getLayers() };
    _tileLayers.clear();

    for (const auto &layer : layers)
    {
        switch (layer->getType())
        {
        case tmx::Layer::Type::Tile:
        {
            _tileLayers.push_back(
                std::make_unique<TileLayer>(map, layer->getLayerAs<tmx::TileLayer>()));
            break;
        }
        case tmx::Layer::Type::Object:
        {
            _objectLayers.push_back(
                std::make_unique<ObjectLayer>(layer->getLayerAs<tmx::ObjectGroup>()));
            break;
        }
        default:
        {
            break;
        }
        }
    }
}

void Controller::handleCollisions() const
{
    for (const auto &objectLayer : _objectLayers)
        for (const auto *object : objectLayer->getObjects("collider"))
            _player->handleCollision(*object);
}

} // namespace Scene::Level
