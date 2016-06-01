//
// Created by Anthony on 5/31/2016.
//

#ifndef ARKANGEL_LINESHAPE_HPP
#define ARKANGEL_LINESHAPE_HPP

#include <sfml/Graphics/RectangleShape.hpp>

class LineShape : public sf::RectangleShape
{
    sf::Vector2f Point1, Point2;
    float Thickness;
    sf::Color FillColor;

public:
    LineShape(sf::Vector2f& point1, sf::Vector2f& point2, float thickness, sf::Color color = sf::Color(255, 255, 255));

    float GetThickness();
    void SetThickness(float thickness);
    sf::Color GetColor();
    void SetColor(sf::Color& color);
};


#endif //ARKANGEL_LINESHAPE_HPP
