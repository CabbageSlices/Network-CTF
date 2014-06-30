#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

extern const float PI;

///all functions are in degrees
float calculateReferenceAngle(const float& angleStandardPosition);

//calculate angle formed between two points, angle is the angle in standard position in degrees
float calculateAngle(const sf::Vector2f& beginPoint, const sf::Vector2f& endPoint);

float degreesToRadians(const float& angleInDegrees);
float radiansToDegrees(const float& angleInRadians);

//returns the squared distance between two points
float distanceToPoint(const sf::Vector2f& beginPoint, const sf::Vector2f& endPoint);

//calculate the center point of a given rectangle
sf::Vector2f calculateCenter(const sf::FloatRect& rect);

#endif // MATH_H_INCLUDED
