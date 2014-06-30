#include "Gun.h"
#include "LineSegment.h"
#include "Bullet.h"
#include <cmath>
#include "math.h"
#include <iostream>

using std::cos;
using std::sin;
using std::tr1::shared_ptr;
using std::vector;

Gun::Gun() :
    lineOfSight(sf::Lines, 2),
    lineTexture(),
    bullets(),
    MAX_DISTANCE_FIRED(2000.f),
    rotation(0.0)
    {
        //set a default location for the line of sight
        updateLineOfSight(sf::Vector2f(0, 0), 0.0);
        lineOfSight[0].texCoords = sf::Vector2f(0, 0);
        lineOfSight[1].texCoords = sf::Vector2f(100, 0);

        lineTexture.loadFromFile("texture.png");
    }

void Gun::updateRotation(const sf::Vector2f& playerPosition, const float& playerRotation) {

    updateLineOfSight(playerPosition, playerRotation);
}

void Gun::draw(sf::RenderWindow& window) {

    window.draw(lineOfSight, &lineTexture);

    for(auto bullet : bullets) {

        bullet->draw(window);
    }
}

vector<shared_ptr<Bullet> > Gun::getBullets() {

    return bullets;
}

float Gun::fire() {

    createBullet();

    //this gun just fires in the same place as its rotation
    return rotation;
}

void Gun::fire(const float& angle) {

    //save the old state of the gun before changing
    //that way it can be restored once gun is fired
    float oldRotation = rotation;
    sf::Vector2f oldEndPoint = lineOfSight[1].position;

    rotation = angle;
    lineOfSight[1].position = calculateEndPoint(lineOfSight[0].position);

    //fire with new angle
    createBullet();

    //restore gun to previous state
    lineOfSight[1].position = oldEndPoint;
    rotation = oldRotation;
}

void Gun::updateBullets(const sf::Time& delta) {

    //update all bullets and remove the ones that are dead
    for(unsigned i = 0; i < bullets.size();) {

        bullets[i]->updateElapsedTime(delta);

        if(!bullets[i]->isAlive()) {

            bullets.erase(bullets.begin() + i);
            continue;
        }

        i++;
    }
}

void Gun::updateLineOfSight(const sf::Vector2f& origin, const float& newRotation) {

    rotation = newRotation;
    lineOfSight[0].position = origin;

    //calculate the end point of the line of sight
    sf::Vector2f endPoint = calculateEndPoint(origin);

    lineOfSight[1].position = endPoint;
}

sf::Vector2f Gun::calculateEndPoint(const sf::Vector2f& beginPoint) const {

    //using the angle you can figure out the x and y components of the
    //triangle formed form the origin to the end point because you know the
    //lenght of the hypotenuse
    float xComponent = cos(degreesToRadians(rotation)) * MAX_DISTANCE_FIRED;

    //in games y is positive towards the bottom and negative towards the top
    //so inverse the sign of the y component because the math calculations assumes
    //y is positive towards the top and negative towards the bottom
    float yComponent = -sin(degreesToRadians(rotation)) * MAX_DISTANCE_FIRED;

    //find final position by addition the components to the initial position
    //because the x and y components are already negative or positive depending on which
    //way the player is aiming in order to reach that position
    sf::Vector2f endPoint(beginPoint.x + xComponent, beginPoint.y + yComponent);

    return endPoint;
}

void Gun::createBullet() {

    //create a line for the bullet
    shared_ptr<LineSegment> bulletLine = createLine(lineOfSight[0].position, lineOfSight[1].position,
                                                    "", sf::Vector2f(0, 0), sf::Vector2f(0, 0));
    shared_ptr<Bullet> bullet(new Bullet(bulletLine));

    bullets.push_back(bullet);
}
