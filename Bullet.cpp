#include "Bullet.h"
#include "LineSegment.h"

using std::tr1::shared_ptr;

Bullet::Bullet(std::tr1::shared_ptr<LineSegment> bulletLine, const unsigned& originFloor) :
    line(bulletLine),
    LIFE_TIME(sf::milliseconds(80)),
    elapsedTime(sf::seconds(0)),
    canCollide(true),
    floor(originFloor)
    {

    }

void Bullet::updateElapsedTime(const sf::Time& deltaTime) {

    elapsedTime += deltaTime;
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

    return 10;
}

const unsigned Bullet::getFloor() const {

    return floor;
}

void Bullet::draw(sf::RenderWindow& window) {

    line->draw(window);
}

void Bullet::setEndPoint(const sf::Vector2f endPoint) {

    line->setEndPoint(endPoint);
}

shared_ptr<LineSegment> Bullet::getLine() {

    return line;
}
