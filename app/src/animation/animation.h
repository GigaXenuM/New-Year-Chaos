#pragma once
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"

class Animation
{
public:
    explicit Animation(std::vector<sf::Texture> &textures, float frameTime = 0.1f);

    void start();
    void stop();
    void update(float deltatime, sf::Sprite &sprite);

private:
    std::vector<sf::Texture> &_textures;
    float _frameTime;
    float _elapsedTime{ 0.f };
    int _currentFrame{ 0 };
    bool _isPlaying{ false };
};