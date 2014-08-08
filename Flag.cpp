#include "Flag.h"
#include "math.h"

Flag::Flag(const sf::Vector2f& spawnLocation, const sf::Color& flagColor):
    flag(),
    spawnPosition(spawnLocation),
    beingHeld(false),
    atSpawn(true)
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

void Flag::reset() {

    atSpawn = true;
    beingHeld = false;
    flag.setPosition(spawnPosition);
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

void Flag::setHeld(bool held) {

    beingHeld = held;
}
