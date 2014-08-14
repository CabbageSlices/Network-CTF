#include "Bullet.h"
#include "LineSegment.h"
#include "math.h"

using std::tr1::shared_ptr;

Bullet::Bullet(std::tr1::shared_ptr<LineSegment> bulletLine, const unsigned& originFloor, const int& bulletDamage) :
    line(bulletLine),
    bulletTracer(sf::Vector2f(25, 2)),
    tracerTravelDistance(0, 0),
    LIFE_TIME(sf::milliseconds(200)),
    elapsedTime(sf::seconds(0)),
    tracerVelocity(0, 0),
    canCollide(true),
    damage(bulletDamage),
    floor(originFloor)
    {
        tracerVelocity.x = (bulletLine->getEndPoint().x - bulletLine->getStartPoint().x) / LIFE_TIME.asSeconds();
        tracerVelocity.y = (bulletLine->getEndPoint().y - bulletLine->getStartPoint().y) / LIFE_TIME.asSeconds();

        bulletTracer.setPosition(bulletLine->getStartPoint());
        bulletTracer.setOrigin(calculateCenter(bulletTracer.getLocalBounds()));
        bulletTracer.setRotation(-calculateAngle(bulletLine->getStartPoint(), bulletLine->getEndPoint()));
        bulletTracer.setFillColor(sf::Color::Yellow);
    }

void Bullet::updateElapsedTime(const sf::Time& deltaTime) {

    elapsedTime += deltaTime;

    //bullet's lifetime is only valid if it has collided already
    //if it hasn't collided then the time shouldn't move
    if(checkCanCollide()) {

        elapsedTime = sf::milliseconds(0);
        return;
    }

    moveTracer(deltaTime);
}

bool Bullet::canDraw() {

    return elapsedTime < LIFE_TIME;
}

bool Bullet::checkCanCollide() {

    return canCollide;
}

void Bullet::disableCollision() {

    canCollide = false;
}

int Bullet::getDamage() {

    return damage;
}

const unsigned Bullet::getFloor() const {

    return floor;
}

void Bullet::draw(sf::RenderWindow& window) {

    if(shouldDrawTracer()) {

       window.draw(bulletTracer);
    }
}

void Bullet::setEndPoint(const sf::Vector2f endPoint) {

    line->setEndPoint(endPoint);
}

shared_ptr<LineSegment> Bullet::getLine() {

    return line;
}

bool Bullet::shouldDrawTracer() {

    return (distanceToPoint(line->getStartPoint(), line->getEndPoint()) > distanceToPoint(sf::Vector2f(0, 0), tracerTravelDistance));
}

void Bullet::moveTracer(const sf::Time& delta) {

    float xOffset = tracerVelocity.x * delta.asSeconds();
    float yOffset = tracerVelocity.y * delta.asSeconds();

    //move the tracer closer to the end point of the bullet
    bulletTracer.move(xOffset, yOffset);

    tracerTravelDistance.x += xOffset;
    tracerTravelDistance.y += yOffset;
}
