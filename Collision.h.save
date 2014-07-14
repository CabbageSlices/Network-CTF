#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "Bullet.h"
#include <tr1/memory>
#include <vector>

class LineSegment;

//check for collision between the rectangle and line and return the point of collision
bool checkCollision(const sf::FloatRect& rect, std::tr1::shared_ptr<LineSegment> line, sf::Vector2f& collisionPoint);

//checks if the point of collision is closer than the distance to the given point and if so
//sets a new nearest point of collision, returns true if there was a collision, false otherwise
bool handleEdgeCollision(const sf::Vector2f& edgeBegin, const sf::Vector2f& edgeEnd, std::tr1::shared_ptr<LineSegment> line, sf::Vector2f& nearestIntersectionPoint);

//calculate the offset required for the first block to no longer be colliding with the second block
sf::Vector2f calculateCollisionOffset(const sf::FloatRect& rectToMove, const sf::FloatRect& staticRect);

//handle collision between the given bullet and all objects in the given container, only the bullet is changed
template<class Entity>
void bulletEntityCollision(std::tr1::shared_ptr<Bullet> bullet, std::vector<std::tr1::shared_ptr<Entity> >& entities) {

    //go through all entities and check for collision with them
    //line cuts off after each collision so in the end it will have collided with the nearest object
    for(auto entity : entities) {

        sf::FloatRect collisionRect = entity->getCollisionBox();

        sf::Vector2f collisionPoint(0, 0);

        if(checkCollision(collisionRect, bullet->getLine(), collisionPoint)) {

            bullet->setEndPoint(collisionPoint);
            bullet->disableCollision();
            continue;
        }
    }
}

#endif // COLLISION_H_INCLUDED
