#include "debugdrawer.h"

#include "util/geometryoperation.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>

namespace Util
{

DebugDrawer::DebugDrawer(sf::RenderTarget *target) : _target{ target }
{
}

void DebugDrawer::DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
    sf::ConvexShape polygon(vertexCount);
    sf::Vector2f center;
    for (int i = 0; i < vertexCount; i++)
    {
        sf::Vector2f transformedVec = Util::b2VecToSFVec(vertices[i]);
        polygon.setPoint(i,
                         sf::Vector2f(std::floor(transformedVec.x), std::floor(transformedVec.y)));
    }
    polygon.setOutlineThickness(-1.f);
    polygon.setFillColor(sf::Color::Transparent);
    polygon.setOutlineColor(Util::glColorToSFML(color));

    _target->draw(polygon);
}
void DebugDrawer::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
    sf::ConvexShape polygon(vertexCount);
    for (int i = 0; i < vertexCount; i++)
    {
        sf::Vector2f transformedVec = Util::b2VecToSFVec(vertices[i]);
        polygon.setPoint(i,
                         sf::Vector2f(std::floor(transformedVec.x), std::floor(transformedVec.y)));
    }
    polygon.setOutlineThickness(-1.f);
    polygon.setFillColor(Util::glColorToSFML(color, 60));
    polygon.setOutlineColor(Util::glColorToSFML(color));

    _target->draw(polygon);
}
void DebugDrawer::DrawCircle(const b2Vec2 &center, float radius, const b2Color &color)
{
    sf::CircleShape circle(radius);
    circle.setOrigin({ radius, radius });
    circle.setPosition(Util::b2VecToSFVec(center));
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineThickness(-1.f);
    circle.setOutlineColor(Util::glColorToSFML(color));

    _target->draw(circle);
}
void DebugDrawer::DrawSolidCircle(const b2Vec2 &center, float radius, const b2Vec2 &axis,
                                  const b2Color &color)
{
    sf::CircleShape circle(radius);
    circle.setOrigin({ radius, radius });
    circle.setPosition(Util::b2VecToSFVec(center));
    circle.setFillColor(Util::glColorToSFML(color, 60));
    circle.setOutlineThickness(1.f);
    circle.setOutlineColor(Util::glColorToSFML(color));

    b2Vec2 endPoint = center + radius * axis;
    sf::Vertex line[2] = {
        sf::Vertex(Util::b2VecToSFVec(center), Util::glColorToSFML(color)),
        sf::Vertex(Util::b2VecToSFVec(endPoint), Util::glColorToSFML(color)),
    };

    _target->draw(circle);
    _target->draw(line, 2, sf::PrimitiveType::Lines);
}
void DebugDrawer::DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color)
{
    sf::Vertex line[] = { sf::Vertex(Util::b2VecToSFVec(p1), Util::glColorToSFML(color)),
                          sf::Vertex(Util::b2VecToSFVec(p2), Util::glColorToSFML(color)) };

    _target->draw(line, 2, sf::PrimitiveType::Lines);
}
void DebugDrawer::DrawTransform(const b2Transform &xf)
{
    float lineLength = 0.4;
    b2Vec2 xAxis = xf.p + lineLength * xf.q.GetXAxis();
    sf::Vertex redLine[] = { sf::Vertex(Util::b2VecToSFVec(xf.p), sf::Color::Red),
                             sf::Vertex(Util::b2VecToSFVec(xAxis), sf::Color::Red) };
    b2Vec2 yAxis = xf.p + lineLength * xf.q.GetYAxis();
    sf::Vertex greenLine[] = { sf::Vertex(Util::b2VecToSFVec(xf.p), sf::Color::Green),
                               sf::Vertex(Util::b2VecToSFVec(yAxis), sf::Color::Green) };

    _target->draw(redLine, 2, sf::PrimitiveType::Lines);
    _target->draw(greenLine, 2, sf::PrimitiveType::Lines);
}

void DebugDrawer::DrawPoint(const b2Vec2 &p, float size, const b2Color &color)
{
}

} // namespace Util
