#include "Collision.h"
#include "LineSegment.h"
#include "math.h"

using std::tr1::shared_ptr;

bool checkCollision(const sf::FloatRect& rect, shared_ptr<LineSegment> line, sf::Vector2f& collisionPoint) {

    //find the point of intersection that is the nearest to the origin of the line because this point would be the first point of collision
    //set default nearest intersection point to the end point of the line because thats the farthest it can go to have an intersection
    sf::Vector2f nearestIntersectionPoint = line->getEndPoint();
    bool intersects = false;

    //top edge
    if(handleEdgeCollision(sf::Vector2f(rect.left, rect.top), sf::Vector2f(rect.left + rect.width, rect.top), line, nearestIntersectionPoint)) {

        intersects = true;
    }

    //left edge
    if(handleEdgeCollision(sf::Vector2f(rect.left, rect.top), sf::Vector2f(rect.left, rect.top + rect.height), line, nearestIntersectionPoint)) {

        intersects = true;
    }

    //right edge
    if(handleEdgeCollision(sf::Vector2f(rect.left + rect.width, rect.top), sf::Vector2f(rect.left + rect.width, rect.top + rect.height), line, nearestIntersectionPoint)) {

        intersects = true;
    }

    //bottom
    if(handleEdgeCollision(sf::Vector2f(rect.left, rect.top + rect.height), sf::Vector2f(rect.left + rect.width, rect.top + rect.height), line, nearestIntersectionPoint)) {

        intersects = true;
    }

    collisionPoint = nearestIntersectionPoint;
    return intersects;
}

bool handleEdgeCollision(const sf::Vector2f& edgeBegin, const sf::Vector2f& edgeEnd, shared_ptr<LineSegment> line, sf::Vector2f& nearestIntersectionPoint) {

    shared_ptr<LineSegment> edge = createLine(edgeBegin, edgeEnd);

    sf::Vector2f collisionPoint(0, 0);

    if(line->intersects(*edge, collisionPoint)) {

        if(distanceToPoint(line->getStartPoint(), collisionPoint) < distanceToPoint(line->getStartPoint(), nearestIntersectionPoint)) {

            nearestIntersectionPoint = collisionPoint;
        }

        return true;
    }

    return false;
}
