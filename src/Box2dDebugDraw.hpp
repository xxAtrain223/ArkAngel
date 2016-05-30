//
// Created by Anthony on 5/27/2016.
//

#ifndef ARKANGEL_BOX2DDEBUGDRAW_HPP
#define ARKANGEL_BOX2DDEBUGDRAW_HPP

#include <Box2D/Box2D.h>
#include <sfml/Graphics.hpp>

class Box2dDebugDraw : public b2Draw
{
    sf::RenderWindow* Window;
    float PixelsPerMeter;
    float MetersPerPixel;

public:
    Box2dDebugDraw(sf::RenderWindow &window, float ratio) :
        Window(&window),
        PixelsPerMeter(ratio),
        MetersPerPixel(1.f / ratio)
    { }

    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
    void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
    void DrawTransform(const b2Transform& xf);
    sf::Color B2SFColor(const b2Color &color, int alpha = 255);

    float getPixelsPerMeter();
    float getMetersPerPixel();
};


#endif //ARKANGEL_BOX2DDEBUGDRAW_HPP
