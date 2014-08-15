#include "Portal.h"
#include "Floors.h"
#include "UserPlayer.h"

Portal::Portal(const sf::Vector2f& centerPosition, const sf::Vector2f& teleportLocation, const unsigned targetFloor):
    StaticObject(centerPosition),
    teleportPosition(sf::Vector2f(50, 50)),
    destinationFloor(targetFloor)
    {
        teleportPosition.setFillColor(sf::Color::Magenta);
        teleportPosition.setPosition(teleportLocation);
    }

Portal::Portal(const sf::Vector2f& centerPosition):
    StaticObject(centerPosition),
    teleportPosition(sf::Vector2f(50, 50)),
    destinationFloor(UNDERGROUND_FLOOR)
    {
        teleportPosition.setFillColor(sf::Color::Magenta);
    }

void Portal::setTeleportPosition(const sf::Vector2f& position, const unsigned targetFloor) {

    teleportPosition.setPosition(position);
    destinationFloor = targetFloor;
}

const sf::Vector2f& Portal::getTeleportPosition() const {

    return teleportPosition.getPosition();
}

const unsigned Portal::getDestinationFloor() const {

    return destinationFloor;
}

void Portal::handleCollision(UserPlayer& collidingPlayer) {

    collidingPlayer.setInterpolationPosition(teleportPosition.getPosition());
    collidingPlayer.setFloor(destinationFloor);
}

void Portal::draw(sf::RenderWindow& window) {

    StaticObject::draw(window);
    window.draw(teleportPosition);
}
