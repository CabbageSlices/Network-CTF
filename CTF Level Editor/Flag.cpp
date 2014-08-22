#include "Flag.h"
#include "Floors.h"
#include "math.h"

Flag::Flag(const sf::Vector2f& spawnLocation, const sf::Color& flagColor):
    flag(),
    spawnPosition(spawnLocation),
    beingHeld(false),
    atSpawn(true),
    floor(OVERGROUND_FLOOR)
    {
        flag.setRadius(20);
        flag.setOrigin(calculateCenter(flag.getLocalBounds()));
        flag.setPosition(spawnPosition);
        flag.setFillColor(flagColor);
    }

void Flag::draw(sf::RenderWindow& window) {

    window.draw(flag);
}

void Flag::setPosition(const sf::Vector2f& position) {

    flag.setPosition(position);
    atSpawn = false;
}

void Flag::setSpawn(const sf::Vector2f& position) {

    spawnPosition = position;

    reset();
}

void Flag::reset() {

    atSpawn = true;
    beingHeld = false;
    flag.setPosition(spawnPosition);
    floor = OVERGROUND_FLOOR;
}

void Flag::dropFlag() {

    beingHeld = false;
}

void Flag::pickUpFlag() {

    beingHeld = true;
}

const bool& Flag::isHeld() const {

    return beingHeld;
}

const bool& Flag::isAtSpawn() const {

    return atSpawn;
}

const sf::FloatRect Flag::getCollisionBox() const {

    return flag.getGlobalBounds();
}

const sf::Vector2f Flag::getPosition() const {

    return flag.getPosition();
}

const unsigned& Flag::getFloor() const {

    return floor;
}

const sf::Vector2f& Flag::getSpawn() const {

    return spawnPosition;
}

void Flag::setHeld(bool held) {

    beingHeld = held;
}

void Flag::setFloor(const unsigned& destinationFloor) {

    floor = destinationFloor;
}
