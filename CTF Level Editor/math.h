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

//floats are equal if they are within a amount of each other
bool compareFloats(const float& float1, const float& float2);

//compare two rectangles, they are the same if all of their positions and sizes are equal
bool compareRects(const sf::FloatRect rect1, const sf::FloatRect rect2);

//normally generate a random number within the range
//[max - 1 + min, min] where the left side is the maximum number generated, and the right side is the minimum
///because of the way the maximum range increases if the minimum is greater than 1
///you must subtract from the minimum range in order to make it equal to 1
///and at the same time subtract the same amount from the maximum
///that way the max range is not changed
///and at the end, add the subtracted value back to the result
///this way the given random number is not greather than the max range and is still within the range [max, min]
int getRand(int max, int min);

#endif // MATH_H_INCLUDED
