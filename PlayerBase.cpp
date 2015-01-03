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
    redIndicatorTexture(),
    blueIndicatorTexture(),
    flagIndicator(),
    playerId(0),
    teamId(1),
    pastHitBox(sf::Vector2f(31, 35)),
    currentHitBox(pastHitBox),
    destinationHitBox(currentHitBox),
    health(80, 4, 2),
    playerTexture(),
    playerSprite(),
    frame(0),
    animationTimer(),
    animationTime(sf::milliseconds(100)),
    clips(),
    canPlayDeathAnimation(true),
    playSpawnAnimation(false),
    spawnTexture(),
    spawnSprite(),
    spawnClips(),
    deathClips(),
    pastRotation(0),
    currentRotation(0),
    destinationRotation(0),
    gun(new Pistol()),
    flagBeingHeld(),
    dataReceiveTimer(),
    maxNoData(sf::seconds(6)),
    respawnTimer(),
    respawnDelay(sf::seconds(7)),
    playerName("Name"),
    kills(0),
    deaths(0),
    flagCaptures(0),
    flagReturns(0),
    maxInterpolationDist(260100),
    currentFloor(OVERGROUND_FLOOR),
    pickingUpGun(false),
    respawnBuffer(),
    respawnSound(),
    getFlagBuffer(),
    getFlagSound(),
    scoreBuffer(),
    scoreSound(),
    shouldPlaySounds(true)
    {
        setupClips();

        //set the origin of the hit boxes to the center because player needs to rotate around the center
        pastHitBox.setOrigin(calculateCenter(pastHitBox.getGlobalBounds() ));
        pastHitBox.setPosition(60, 60);
        currentHitBox = pastHitBox;
        destinationHitBox = pastHitBox;

        currentHitBox.setOutlineThickness(4.0);
        currentHitBox.setFillColor(sf::Color::Transparent);

        pastHitBox.setFillColor(sf::Color(0, 0, 255, 100));
        destinationHitBox.setFillColor(sf::Color(255, 0, 0, 100));

        playerTexture.loadFromFile("character.png");
        playerSprite.setTexture(playerTexture);

        //default center based on image file of the character
        playerSprite.setOrigin(sf::Vector2f(22, 26));

        redIndicatorTexture.loadFromFile("redFlagIndicator.png");
        blueIndicatorTexture.loadFromFile("blueFlagIndicator.png");

        respawnBuffer.loadFromFile("sounds/respawn.wav");
        respawnSound.setBuffer(respawnBuffer);

        getFlagBuffer.loadFromFile("sounds/flagPickup.wav");
        getFlagSound.setBuffer(getFlagBuffer);

        scoreBuffer.loadFromFile("sounds/flagSound.wav");
        scoreSound.setBuffer(scoreBuffer);

        spawnTexture.loadFromFile("spawn Animation.png");

        spawnSprite.setTexture(spawnTexture);
        spawnSprite.setTextureRect(spawnClips[0]);
        spawnSprite.setOrigin(spawnSprite.getGlobalBounds().width / 2, spawnSprite.getGlobalBounds().height / 2);
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

    //if the new team isn't the same as the current team then the player nee
    teamId = team;

    //set the appropriate flag indicator
    if(team == TEAM_B_ID) {

        flagIndicator.setTexture(redIndicatorTexture);

    } else {

        flagIndicator.setTexture(blueIndicatorTexture);
    }

    flagIndicator.setScale(1.20, 1.20);

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

    //update position of spawn sprite since its a seperate thing
    spawnSprite.setPosition(currentHitBox.getPosition());

    //set the position of the flag indicator now
    //position it over the health as well, so move it up by 10 pixels
    flagIndicator.setPosition(currentHitBox.getPosition().x - 14, currentHitBox.getGlobalBounds().top - flagIndicator.getGlobalBounds().height - 29);
}

void PlayerBase::stopInterpolation() {

    destinationHitBox.setPosition(currentHitBox.getPosition());
    pastHitBox.setPosition(currentHitBox.getPosition());
    destinationRotation = pastRotation;
}

void PlayerBase::animate() {

    //if the player is running the dying animation then don't handle the animations as normal
    if(getDrawingState() == DYING || getDrawingState() == SPAWNING) {

        if(animationTimer.getElapsedTime() > animationTime) {

            frame += 1;

            if(getDrawingState() == SPAWNING && frame >= spawnClips.size()) {

                playSpawnAnimation = false;
                frame = 0;

                //set the current clip onto the sprite in order to draw it
                playerSprite.setTextureRect(clips[teamId][frame]);

                return;
            }

            animationTimer.restart();
        }

        //if the death animation can play it means the death animation hasn't started yet, and if the frame is greather than 0 it will draw some weird frame so reset frame
        //but only do this if he isn't spawning because when he is spawning he can play the death animation
        if(canPlayDeathAnimation && getDrawingState() != SPAWNING) {

            frame = 0;
        }

        if(frame < spawnClips.size()) {

            if(getDrawingState() == DYING) {

                spawnSprite.setTextureRect(deathClips[frame]);

            } else {

                spawnSprite.setTextureRect(spawnClips[frame]);
            }
        }

        return;
    }


    if(animationTimer.getElapsedTime() > animationTime) {

        frame += 1;

        if(frame > clips[TEAM_A_ID].size() - 1) {

            frame = 0;
        }

        animationTimer.restart();
    }

    //if player is standing still always set the frame to 0
    if(getDrawingState() == STANDING) {

        frame = 0;
    }

    //set the current clip onto the sprite in order to draw it
    playerSprite.setTextureRect(clips[teamId][frame]);
}

void PlayerBase::draw(sf::RenderWindow& window, const unsigned& drawingFloor) {

    //update health position justb efore drawing because it could have moved
    updateHealthPosition();

    //draw the flag indicator if he is holding a flag
    if(flagBeingHeld.lock()) {

        window.draw(flagIndicator);
    }

    //dont draw the normal sprite if player is dying
    if(getDrawingState() != DYING && getDrawingState() != SPAWNING) {

        window.draw(playerSprite);
        this->drawGun(window, drawingFloor);
        health.draw(window);

    } else {

        //if the current frame exceeds the number of frames in the naimtion then don't draw the player at all
        if(frame < spawnClips.size()) {

            window.draw(spawnSprite);
        }
    }

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

    if(shouldPlaySounds && GLO_PLAY_SOUNDS) {

        respawnSound.play();
    }

    frame = 0;
    animationTimer.restart();

    //allow death animtion to play once player dies
    canPlayDeathAnimation = true;

    //start spawn animation
    playSpawnAnimation = true;
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

    if(shouldPlaySounds && GLO_PLAY_SOUNDS) {

        getFlagSound.play();
    }
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

void PlayerBase::resetStats() {

    kills = 0;
    deaths = 0;
    flagCaptures = 0;
    flagReturns = 0;
}

void PlayerBase::resetGun() {

    gun = shared_ptr<Gun>(new Pistol());
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

void PlayerBase::setupClips() {

    //all positions are based on the image of the player sprite itself
    clips[TEAM_B_ID] = vector<sf::IntRect>();

    clips[TEAM_B_ID].push_back(sf::IntRect(3, 3, 59, 43));
    clips[TEAM_B_ID].push_back(sf::IntRect(66, 3, 59, 43));
    clips[TEAM_B_ID].push_back(sf::IntRect(129, 3, 59, 43));
    clips[TEAM_B_ID].push_back(sf::IntRect(192, 3, 59, 43));
    clips[TEAM_B_ID].push_back(sf::IntRect(255, 3, 59, 43));
    clips[TEAM_B_ID].push_back(sf::IntRect(318, 3, 59, 43));
    clips[TEAM_B_ID].push_back(sf::IntRect(381, 3, 59, 43));
    clips[TEAM_B_ID].push_back(sf::IntRect(444, 3, 59, 43));

    clips[TEAM_A_ID] = vector<sf::IntRect>();

    clips[TEAM_A_ID].push_back(sf::IntRect(3, 50, 59, 43));
    clips[TEAM_A_ID].push_back(sf::IntRect(66, 50, 59, 43));
    clips[TEAM_A_ID].push_back(sf::IntRect(129, 50, 59, 43));
    clips[TEAM_A_ID].push_back(sf::IntRect(192, 50, 59, 43));
    clips[TEAM_A_ID].push_back(sf::IntRect(255, 50, 59, 43));
    clips[TEAM_A_ID].push_back(sf::IntRect(318, 50, 59, 43));
    clips[TEAM_A_ID].push_back(sf::IntRect(381, 50, 59, 43));
    clips[TEAM_A_ID].push_back(sf::IntRect(444, 50, 59, 43));

    deathClips.push_back(sf::IntRect(2, 1, 37, 65));
    deathClips.push_back(sf::IntRect(41, 1, 37, 65));
    deathClips.push_back(sf::IntRect(80, 1, 37, 65));
    deathClips.push_back(sf::IntRect(119, 1, 37, 65));
    deathClips.push_back(sf::IntRect(158, 1, 37, 65));
    deathClips.push_back(sf::IntRect(197, 1, 37, 65));
    deathClips.push_back(sf::IntRect(236, 1, 37, 65));

    //spawn animation is the death animation played in reverse so just reverse the death clips
    for(int i = deathClips.size() - 1; i >= 0; --i) {

        spawnClips.push_back(deathClips[i]);
    }
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

    healthPosition.x = currentHitBox.getGlobalBounds().left - currentHitBox.getGlobalBounds().width + 20;
    healthPosition.y = currentHitBox.getGlobalBounds().top - 13;

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

    if(shouldPlaySounds && GLO_PLAY_SOUNDS) {

        respawnSound.play();
    }

    //start the death animation if it hasn't already started
    ///because the die functon is called whenever hte player has 0 health this function will always be called on the client after each server update if the health is 0
    ///so only reset the death animation after he spawns
    if(canPlayDeathAnimation) {

        frame = 0;
        animationTimer.restart();
        canPlayDeathAnimation = false;
    }
}
