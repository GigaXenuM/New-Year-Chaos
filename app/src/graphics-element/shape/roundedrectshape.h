#pragma once

#include <SFML/Graphics.hpp>

namespace Graphics
{

class RoundedRectShape : public sf::Shape
{
public:
    explicit RoundedRectShape(const sf::Vector2f &size = {}, float radius = 0,
                              std::size_t pointCount = 15);

    void setSize(const sf::Vector2f &size);
    const sf::Vector2f &getSize() const;
    void setRadius(float radius);
    float getRadius() const;
    void setCornerPointCount(std::size_t count);

    virtual std::size_t getPointCount() const override;
    virtual sf::Vector2f getPoint(std::size_t index) const override;

private:
    sf::Vector2f _size;            // Size of the rectangle
    float _radius;                 // Radius of the corners
    std::size_t _cornerPointCount; // Number of points per corner
};

} // namespace Graphics
