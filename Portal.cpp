#include "Portal.h"
#include "Floors.h"
#include "UserPlayer.h"

Portal::Portal(const sf::Vector2f& position, const sf::Vector2f& size):
    StaticObject(position),
    teleportPosition(sf::Vector2f(1, 1)),
    destinationFloor(UNDERGROUND_FLOOR)
    {
        collisionBox.setSize(size);
        teleportPosition.setFillColor(sf::Color::Magenta);
    }

void Portal::setTeleportPosition(const sf::Vector2f& position, const sf::Vector2f& size, const unsigned targetFloor) {

    teleportPosition.setPosition(position);
    teleportPosition.setSize(size);
    destinationFloor = targetFloor;
}

void Portal::setDestinationFloor(const unsigned& floor) {

    destinationFloor = floor;
}

const sf::Vector2f& Portal::getTeleportPosition() const {

    return teleportPosition.getPosition();
}

const unsigned Portal::getDestinationFloor() const {

    return destinationFloor;
}

bool Portal::contains(const sf::Vector2f& point) const {

    return StaticObject::contains(point) || teleportPosition.getGlobalBounds().contains(point);
}

void Portal::draw(sf::RenderWindow& window, unsigned floor) {

    StaticObject::draw(window);

    drawDestination(window, floor);
}

void Portal::drawDestination(sf::RenderWindow& window, unsigned floor) {

    //only draw the destination position if its on the floor given
    if(floor == destinationFloor) {

        window.draw(teleportPosition);
    }
}

void Portal::handleCollision(UserPlayer& collidingPlayer) {

    collidingPlayer.setInterpolationPosition(teleportPosition.getPosition());
    collidingPlayer.setFloor(destinationFloor);
}
