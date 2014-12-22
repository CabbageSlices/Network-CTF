#include "PlayerBase.h"
#include "Lerp.h"
#include "math.h"
#include "Flag.h"
#include "Sniper.h"
#include "Pistol.h"
#include "Shotgun.h"
#include "Floors.h"
#include "TeamManager.h"

#include <iostream>

using std::vector;
using std::tr1::shared_ptr;
using std::cout;
using std::endl;


PlayerBase::PlayerBase():
    playerId(0),
    teamId(1),
    pastHitBox(sf::Vector2f(29, 35)),
    currentHitBox(pastHitBox),
    destinationHitBox(currentHitBox),
    health(),
    playerTexture(),
    playerSprite(),
    pastRotation(0),
    currentRotation(0),
    destinationRotation(0),
    gun(new Pistol()),
    flagBeingHeld(),
    dataReceiveTimer(),
    maxNoData(sf::seconds(3)),
    respawnTimer(),
    respawnDelay(sf::seconds(7)),
    playerName("Name"),
    kills(0),
    deaths(0),
    flagCaptures(0),
    flagReturns(0),
    maxInterpolationDist(260100),
    currentFloor(OVERGROUND_FLOOR)
    {

        //set the origin of the hit boxes to the center because player needs to rotate around the center
        pastHitBox.setOrigin(calculateCenter(pastHitBox.getGlobalBounds() ));
        pastHitBox.setPosition(60, 60);
        currentHitBox = pastHitBox;
        destinationHitBox = pastHitBox;

        currentHitBox.setOutlineThickness(4.0);
        currentHitBox.setFillColor(sf::Color::Transparent);

        pastHitBox.setFillColor(sf::Color(0, 0, 255, 100));
        destinationHitBox.setFillColor(sf::Color(255, 0, 0, 100));

        playerTexture.loadFromFile("player.png");
        playerSprite.setTexture(playerTexture);
        playerSprite.setOrigin(calculateCenter(playerSprite.getGlobalBounds() ));
    }

PlayerBase::~PlayerBase() {

    dropFlag();
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

    currentHitBox.setOutlineColor(getTeamColor(team));
    currentHitBox.setFillColor(getTeamColor(team));
}

const float PlayerBase::getRotation() const {

    return currentRotation;
}

void PlayerBase::updateGun(const float& delta) {

    gun->updateGunfire(sf::seconds(delta));
    gun->updateBullets(sf::seconds(delta));
    gun->animate();
}

void PlayerBase::setInterpolationPosition(const sf::Vector2f& position, bool resetData) {

    pastHitBox.setPosition(destinationHitBox.getPosition());
    destinationHitBox.setPosition(position);

    //if the distance to interpolate is too large just set the position to the given position
    //only past hitbox's position needs to be forcibly set because it interpolates from the past hitbox
    if(distanceToPoint(position, pastHitBox.getPosition()) > maxInterpolationDist) {

        pastHitBox.setPosition(position);
    }

    if(resetData) {

        //if interpolation position is set it means its data sent from the server so restart the data recieve timer so player doens't time out
        resetDataTimer();
    }
}

void PlayerBase::setInterpolationPositionV2(const sf::Vector2f& position) {

    pastHitBox.setPosition(currentHitBox.getPosition());

    destinationHitBox.setPosition(position);

    //if the distance to interpolate is too large just set the position to the given position
    //only past hitbox's position needs to be forcibly set because it interpolates from the past hitbox
    if(distanceToPoint(position, pastHitBox.getPosition()) > maxInterpolationDist) {

        pastHitBox.setPosition(position);
    }

    //if interpolation position is set it means its data sent from the server so restart the data recieve timer so player doens't time out
    resetDataTimer();
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

    //if the delta fraction is close enough to one, then interpolation is finished so set all attributes to their target attributes
    if(compareFloats(deltaFraction, 1.0, 0.05)) {

        currentRotation = destinationRotation;
        pastRotation = destinationRotation;
        setPosition(destinationHitBox.getPosition());
        currentHitBox.setPosition(destinationHitBox.getPosition());
    }

    updateHitboxRotation();
    gun->updateRotation(currentHitBox.getPosition(), currentRotation);
}

void PlayerBase::stopInterpolation() {

    destinationHitBox.setPosition(currentHitBox.getPosition());
    pastHitBox.setPosition(currentHitBox.getPosition());
    destinationRotation = pastRotation;
}

void PlayerBase::draw(sf::RenderWindow& window, const unsigned& drawingFloor) {

    //update health position justb efore drawing because it could have moved
    updateHealthPosition();

    this->drawGun(window, drawingFloor);

    window.draw(currentHitBox);
    window.draw(playerSprite);

    health.draw(window);
}

void PlayerBase::drawMinimap(sf::RenderWindow& window) {

    //for now just draw the hitbox on minimap
    window.draw(currentHitBox);
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

bool PlayerBase::shouldRespawn() const {

    return respawnTimer.getElapsedTime() > respawnDelay;
}

void PlayerBase::respawn(const sf::Vector2f& spawnPosition) {

    setPosition(spawnPosition);
    health.refillHealth();
}

void PlayerBase::getHit(int damage) {

    setHealth(health.getCurrentHealth() - damage);
}

void PlayerBase::setHealth(int value) {

    //check if health changed at all
    //that way if his health reaches 0 you know for sure it was because the value was changed
    if(value == health.getCurrentHealth()) {

        //no change
        return;
    }

    //check if given value is less than current health
    if(value < health.getCurrentHealth()) {

        ///play damage animation
    }

    health.setCurrentHealth(value);

    if(health.getCurrentHealth() == 0) {

        die();
    }
}

bool PlayerBase::isHoldingFlag() const {

    return flagBeingHeld.lock();
}

void PlayerBase::holdFlag(shared_ptr<Flag> flagToHold) {

    flagBeingHeld = flagToHold;
    flagToHold->pickUpFlag();
}

void PlayerBase::dropFlag() {

    //drop the flag at his current position
    dropFlag(getDestinationPosition());
}

void PlayerBase::setFloor(const unsigned& destinationFloor) {

    currentFloor = destinationFloor;
    gun->setFloor(destinationFloor);
}

const unsigned PlayerBase::getFloor() const {

    return currentFloor;
}

void PlayerBase::dropFlag(const sf::Vector2f& positionToDrop) {

    //if he has a flag then drop it, also set its last position to the given position
    if(flagBeingHeld.lock()) {

        flagBeingHeld.lock()->setPosition(positionToDrop);
        flagBeingHeld.lock()->setFloor(getFloor());
        flagBeingHeld.lock()->dropFlag();
    }

    flagBeingHeld.reset();
}

bool PlayerBase::timedOut() {

    return dataReceiveTimer.getElapsedTime() > maxNoData;
}

void PlayerBase::resetDataTimer() {

    dataReceiveTimer.restart();
}

shared_ptr<Gun> PlayerBase::getGun() {

    return gun;
}

void PlayerBase::setGun(shared_ptr<Gun> newGun) {

    gun = newGun;
}

const std::string& PlayerBase::getName() const {

    return playerName;
}

const unsigned short& PlayerBase::getKills() const {

    return kills;
}

const unsigned short& PlayerBase::getDeaths() const {

    return deaths;
}

const unsigned short& PlayerBase::getFlagCaptures() const {

    return flagCaptures;
}

const unsigned short& PlayerBase::getFlagReturns() const {

    return flagReturns;
}

void PlayerBase::setName(const std::string& name) {

    playerName = name;
}

void PlayerBase::setKills(const unsigned short& amount) {

    kills = amount;
}

void PlayerBase::setDeaths(const unsigned short& amount) {

    deaths = amount;
}

void PlayerBase::setCaptures(const unsigned short& amount) {

    flagCaptures = amount;
}

void PlayerBase::setReturns(const unsigned short& amount) {

    flagReturns = amount;
}

void PlayerBase::setPosition(const sf::Vector2f& position) {

    pastHitBox.setPosition(position);
    destinationHitBox.setPosition(position);
    currentHitBox.setPosition(position);
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

    //stop interpolating completely
    stopInterpolation();

    respawnTimer.restart();

    //when he respawns gun should be reloaded
    gun->refillTotalAmmo();

    //increase the amount of deaths
    setDeaths(getDeaths() + 1);
}
