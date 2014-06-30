#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include <tr1/memory>

class LineSegment;

//check for collision between the rectangle and line and return the point of collision
bool checkCollision(const sf::FloatRect& rect, std::tr1::shared_ptr<LineSegment> line, sf::Vector2f& collisionPoint);

//checks if the point of collision is closer than the distance to the given point and if so
//sets a new nearest point of collision, returns true if there was a collision, false otherwise
bool handleEdgeCollision(const sf::Vector2f& edgeBegin, const sf::Vector2f& edgeEnd, std::tr1::shared_ptr<LineSegment> line, sf::Vector2f& nearestIntersectionPoint);


#endif // COLLISION_H_INCLUDED
