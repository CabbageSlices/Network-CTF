#include "Flag.h"
#include "Floors.h"
#include "math.h"

#include <iostream>

using std::cout;
using std::endl;

Flag::Flag(const sf::Vector2f& spawnLocation, const std::string& flagTexturePath):
    texture(),
    sprite(),
    spawnPosition(spawnLocation),
    beingHeld(false),
    atSpawn(true),
    floor(OVERGROUND_FLOOR)
    {
        texture.loadFromFile(flagTexturePath);

        sprite.setTexture(texture);
        sprite.setOrigin(calculateCenter(sprite.getGlobalBounds()));

        sprite.setPosition(spawnPosition);
    }

void Flag::draw(sf::RenderWindow& window) {

    window.draw(sprite);
}

void Flag::setPosition(const sf::Vector2f& position) {

    sprite.setPosition(position);
    atSpawn = false;
}

void Flag::setSpawn(const sf::Vector2f& position) {

    spawnPosition = position;

    reset();
}

void Flag::reset() {

    atSpawn = true;
    beingHeld = false;
    sprite.setPosition(spawnPosition);
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

    return sprite.getGlobalBounds();
}

const sf::Vector2f Flag::getPosition() const {

    return sprite.getPosition();
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
