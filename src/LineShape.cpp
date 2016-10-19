//
// Created by Anthony on 5/31/2016.
//

#include "LineShape.hpp"

#include <cmath>

LineShape::LineShape(sf::Vector2f &point1, sf::Vector2f &point2, float thickness, sf::Color color) :
    Point1(point1),
    Point2(point2),
    Thickness(thickness)
{
    sf::Vector2f delta(Point2 - Point1);
    float distance = (float)sqrt(delta.x * delta.x + delta.y * delta.y);
    setSize(sf::Vector2f(distance, thickness));
    setOrigin(sf::Vector2f(0, thickness / 2.f));
    setPosition(point1);
    setRotation((float)atan2(delta.y, delta.x) * 180.f / 3.14159265f);
    setFillColor(color);
}

float LineShape::GetThickness()
{
    return Thickness;
}

void LineShape::SetThickness(float thickness)
{
    Thickness = thickness;
    setSize(sf::Vector2f(getLocalBounds().width, thickness));
    setOrigin(sf::Vector2f(0, thickness / 2.f));
    setPosition(Point1);
}

sf::Color LineShape::GetColor()
{
    return FillColor;
}

void LineShape::SetColor(sf::Color &color)
{
    FillColor = color;
    setFillColor(FillColor);
}
