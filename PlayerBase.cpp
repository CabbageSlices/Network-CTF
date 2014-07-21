#include "PlayerBase.h"
#include "Lerp.h"
#include "math.h"

#include <iostream>

using std::vector;
using std::tr1::shared_ptr;
using std::cout;
using std::endl;

#include "Gun.h"

PlayerBase::PlayerBase():
    playerId(0),
    pastHitBox(sf::Vector2f(29, 35)),
    currentHitBox(pastHitBox),
    destinationHitBox(currentHitBox),
    health(),
    playerTexture(),
    playerSprite(),
    pastRotation(0),
    currentRotation(0),
    destinationRotation(0),
    gun(new Gun())
    {
        //set the origin of the hit boxes to the center because player needs to rotate around the center
        pastHitBox.setOrigin(calculateCenter(pastHitBox.getGlobalBounds() ));
        currentHitBox = pastHitBox;
        destinationHitBox = pastHitBox;

        playerTexture.loadFromFile("player.png");
        playerSprite.setTexture(playerTexture);
        playerSprite.setOrigin(calculateCenter(playerSprite.getGlobalBounds() ));
    }

vector<shared_ptr<Bullet> > PlayerBase::getBullets() {

    return gun->getBullets();
}

int PlayerBase::getId() {

    return playerId;
}

void PlayerBase::setId(const int& id) {

    playerId = id;
}

const float PlayerBase::getRotation() const {

    return currentRotation;
}

void PlayerBase::updateGun(const float& delta) {

    gun->updateGunfire(sf::seconds(delta));
    gun->updateBullets(sf::seconds(delta));
}

void PlayerBase::setInterpolationPosition(const sf::Vector2f& position) {

    currentHitBox.setPosition(pastHitBox.getPosition());
    pastHitBox.setPosition(destinationHitBox.getPosition());
    destinationHitBox.setPosition(position);
}

void PlayerBase::interpolate(const float& deltaFraction) {

    //interpolate the position first
    //calculate how far player should in between the past and destination hit box and set his position to that
    const float& horizontalDistance = ::interpolate(pastHitBox.getPosition().x, destinationHitBox.getPosition().x, deltaFraction);
    const float& verticalDistance = ::interpolate(pastHitBox.getPosition().y, destinationHitBox.getPosition().y, deltaFraction);

    currentHitBox.setPosition(pastHitBox.getPosition().x + horizontalDistance, pastHitBox.getPosition().y + verticalDistance);

    //now update the sprites position
    updateSpritePosition();

    //interpolate the player's rotation
    float rotationDelta = ::interpolate(pastRotation, destinationRotation, deltaFraction);
    currentRotation = pastRotation + rotationDelta;

    updateHitboxRotation();
    gun->updateRotation(currentHitBox.getPosition(), currentRotation);
}

void PlayerBase::draw(sf::RenderWindow& window) {

    //update health position justb efore drawing because it could have moved
    updateHealthPosition();

    this->drawGun(window);

    window.draw(currentHitBox);

    window.draw(playerSprite);

    health.draw(window);
}

const sf::Vector2f& PlayerBase::getDestinationPosition() const {

    return destinationHitBox.getPosition();
}

sf::FloatRect PlayerBase::getCurrentHitbox() const {

    return currentHitBox.getGlobalBounds();
}

sf::FloatRect PlayerBase::getCollisionBox() const {

    return currentHitBox.getGlobalBounds();
}

sf::FloatRect PlayerBase::getDestinationBox() const {

    return destinationHitBox.getGlobalBounds();
}

void PlayerBase::move(const sf::Vector2f& offset) {

    destinationHitBox.move(offset);
}

int PlayerBase::getHealth() const {

    return health.getCurrentHealth();
}

void PlayerBase::getHit(int damage) {

    setHealth(health.getCurrentHealth() - damage);
}

void PlayerBase::setHealth(int value) {

    //check if given value is less than current health
    if(value < health.getCurrentHealth()) {

        ///play damage animation
    }

    health.setCurrentHealth(value);
}

void PlayerBase::updateHitboxRotation() {

    //multiply the rotation by negative one because
    //when drawing the rotation increases clockwise but
    //when doing the calculations the rotations increase counterclockwise
    //so sync the drawing with the calculations

    playerSprite.setRotation(-currentRotation);
}

void PlayerBase::updateHealthPosition() {

    //set the healthbar above the player's current position
    sf::Vector2f healthPosition(0, 0);

    healthPosition.x = currentHitBox.getGlobalBounds().left - currentHitBox.getGlobalBounds().width;
    healthPosition.y = currentHitBox.getGlobalBounds().top - 10;

    health.setPosition(healthPosition);
}

void PlayerBase::updateSpritePosition() {

    playerSprite.setPosition(currentHitBox.getPosition());
}
