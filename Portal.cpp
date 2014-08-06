#include "Portal.h"

Portal::Portal(const sf::Vector2f& centerPosition, const sf::Vector2f& teleportLocation):
    StaticObject(centerPosition),
    teleportPosition(sf::Vector2f(50, 50))
    {
        teleportPosition.setFillColor(sf::Color::Magenta);
        teleportPosition.setPosition(teleportLocation);
    }

Portal::Portal(const sf::Vector2f& centerPosition):
    StaticObject(centerPosition),
    teleportPosition(sf::Vector2f(50, 50))
    {
        teleportPosition.setFillColor(sf::Color::Magenta);
    }

void Portal::setTeleportPosition(const sf::Vector2f& position) {

    teleportPosition.setPosition(position);
}

const sf::Vector2f& Portal::getTeleportPosition() const {

    return teleportPosition.getPosition();
}

void Portal::draw(sf::RenderWindow& window) {

    StaticObject::draw(window);
    window.draw(teleportPosition);
}
