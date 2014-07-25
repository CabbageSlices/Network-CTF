#include "PlayerBase.h"
#include "Lerp.h"
#include "math.h"
#include "Flag.h"
#include "Gun.h"

#include <iostream>

using std::vector;
using std::tr1::shared_ptr;
using std::cout;
using std::endl;


PlayerBase::PlayerBase():
    playerId(0),
    teamId(0),
    pastHitBox(sf::Vector2f(29, 35)),
    currentHitBox(pastHitBox),
    destinationHitBox(currentHitBox),
    health(),
    playerTexture(),
    playerSprite(),
    pastRotation(0),
    currentRotation(0),
    destinationRotation(0),
    gun(new Gun()),
    flagBeingHeld()
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

unsigned short PlayerBase::getTeam() const {

    return teamId;
}

void PlayerBase::setTeam(const unsigned short& team) {

    teamId = team;
}

const float PlayerBase::getRotation() const {

    return currentRotation;
}

void PlayerBase::updateGun(const float& delta) {

    gun->updateGunfire(sf::seconds(delta));
    gun->updateBullets(sf::seconds(delta));
}

void PlayerBase::setInterpolationPosition(const sf::Vector2f& position) {

    pastHitBox.setPosition(currentHitBox.getPosition());
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

const sf::Vector2f& PlayerBase::getCurrentPosition() const {

    return currentHitBox.getPosition();
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

bool PlayerBase::isAlive() const {

    return health.getCurrentHealth() > 0;
}

void PlayerBase::getHit(int damage) {

    setHealth(health.getCurrentHealth() - damage);
}

void PlayerBase::setHealth(int value) {

    //save the current health to check if it actually changed
    int currentHealth = health.getCurrentHealth();

    //check if given value is less than current health
    if(value < currentHealth) {

        ///play damage animation
    }

    health.setCurrentHealth(value);

    //if he has no health and it was because of getting hit, then die
    if(health.getCurrentHealth() == 0 && health.getCurrentHealth() != currentHealth) {

        die();
    }
}

bool PlayerBase::isHoldingFlag() const {

    return flagBeingHeld.lock();
}

void PlayerBase::holdFlag(shared_ptr<Flag> flagToHold) {

    flagBeingHeld = flagToHold;
}

void PlayerBase::dropFlag() {

    //drop the flag at his current position
    dropFlag(getCurrentPosition());
}

void PlayerBase::dropFlag(const sf::Vector2f& positionToDrop) {

    //if he has a flag then drop it, also set its last position to the given position
    if(flagBeingHeld.lock()) {

        flagBeingHeld.lock()->setPosition(positionToDrop);
        flagBeingHeld.lock()->dropFlag();
    }

    flagBeingHeld.reset();
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

void PlayerBase::die() {

    dropFlag();
}
