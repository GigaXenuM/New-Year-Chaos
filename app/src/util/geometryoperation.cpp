#include "geometryoperation.h"

#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Shape.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <box2d/b2_draw.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_math.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/box2d.h>

#include <mapbox/earcut.hpp>

#include <array>
#include <vector>

namespace Util
{

sf::Vector2f b2VecToSFVec(const b2Vec2 &vector)
{
    return sf::Vector2f(vector.x, vector.y);
}

b2Vec2 sfVecToB2Vec(sf::Vector2f vector)
{
    return b2Vec2(vector.x, vector.y);
}

sf::Color glColorToSFML(const b2Color &color, uint8 alpha)
{
    return sf::Color(static_cast<uint8>(color.r * 255), static_cast<uint8>(color.g * 255),
                     static_cast<uint8>(color.b * 255), alpha);
}

void createComplexFixture(b2Body *body, const sf::Shape *shape, b2FixtureDef *bodyDefinition)
{
    using Point = std::array<float, 2>;
    using Polygon = std::vector<Point>;
    using Polygons = std::vector<Polygon>;

    // Handle CircleShape
    if (const auto *circle = dynamic_cast<const sf::CircleShape *>(shape))
    {
        b2CircleShape circleShape;
        circleShape.m_radius = circle->getRadius();

        // Transform the circle's position
        const sf::Transform transform = circle->getTransform();
        const sf::Vector2f transformedCenter = transform.transformPoint(circle->getOrigin());
        circleShape.m_p.Set(transformedCenter.x, transformedCenter.y);

        bodyDefinition->shape = &circleShape;
        b2Fixture *fixture = body->CreateFixture(bodyDefinition);
        fixture->GetUserData().pointer = body->GetUserData().pointer;
        return;
    }

    // Handle RectangleShape
    if (const auto *rectangle = dynamic_cast<const sf::RectangleShape *>(shape))
    {
        b2PolygonShape rectangleShape;

        // Get rectangle's dimensions
        const sf::Vector2f size = rectangle->getSize();
        const sf::Transform transform = rectangle->getTransform();

        // Transform rectangle corners to world coordinates
        b2Vec2 vertices[4];
        vertices[0]
            = b2Vec2(transform.transformPoint(0.f, 0.f).x, transform.transformPoint(0.f, 0.f).y);
        vertices[1] = b2Vec2(transform.transformPoint(size.x, 0.f).x,
                             transform.transformPoint(size.x, 0.f).y);
        vertices[2] = b2Vec2(transform.transformPoint(size.x, size.y).x,
                             transform.transformPoint(size.x, size.y).y);
        vertices[3] = b2Vec2(transform.transformPoint(0.f, size.y).x,
                             transform.transformPoint(0.f, size.y).y);

        rectangleShape.Set(vertices, 4);
        bodyDefinition->shape = &rectangleShape;
        b2Fixture *fixture = body->CreateFixture(bodyDefinition);
        fixture->GetUserData().pointer = body->GetUserData().pointer;
        return;
    }

    // Handle general polygon shapes (e.g., ConvexShape)
    Polygons polygons;
    Polygon polygon;

    // Convert vertices to Earcut-compatible format
    const size_t pointCount = shape->getPointCount();
    polygon.reserve(pointCount);
    const sf::Transform transform = shape->getTransform();
    for (size_t i = 0; i < pointCount; ++i)
    {
        const sf::Vector2f point = transform.transformPoint(shape->getPoint(i));
        polygon.push_back({ point.x, point.y });
    }
    polygons.push_back(polygon);

    // Decompose into triangles using Earcut
    std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(polygons);

    // Create Box2D fixtures for each triangle
    for (size_t i = 0; i < indices.size(); i += 3)
    {
        b2Vec2 triangleVertices[3];
        for (size_t j = 0; j < 3; ++j)
        {
            const auto &pt = polygons[0][indices[i + j]];
            triangleVertices[j] = b2Vec2(pt[0], pt[1]);
        }

        b2PolygonShape triangleShape;
        triangleShape.Set(triangleVertices, 3);

        bodyDefinition->shape = &triangleShape;
        b2Fixture *fixture = body->CreateFixture(bodyDefinition);
        fixture->GetUserData().pointer = body->GetUserData().pointer;
    }
}

sf::ConvexShape convertBodyToSFMLShape(const b2Body *body)
{
    std::vector<b2Vec2> worldVertices;

    // Iterate through all fixtures of the body
    for (const b2Fixture *fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
    {
        const b2Shape *shape = fixture->GetShape();
        if (shape->GetType() == b2Shape::e_polygon)
        {
            const b2PolygonShape *polygonShape = static_cast<const b2PolygonShape *>(shape);
            for (int32 i = 0; i < polygonShape->m_count; ++i)
                worldVertices.push_back(body->GetWorldPoint(polygonShape->m_vertices[i]));
        }
    }

    sf::ConvexShape sfShape;
    sfShape.setPointCount(worldVertices.size());

    for (std::size_t i = 0; i < worldVertices.size(); ++i)
        sfShape.setPoint(i, sf::Vector2f(worldVertices[i].x, worldVertices[i].y));

    return sfShape;
}

} // namespace Util
