#include "Gun.h"
#include "LineSegment.h"
#include "Bullet.h"
#include <cmath>
#include "math.h"
#include "Floors.h"
#include <iostream>

using std::cos;
using std::sin;
using std::tr1::shared_ptr;
using std::vector;
using std::cout;
using std::endl;

Gun::Gun(const int& damage, const float& maxDist, const sf::Time& firingDelay, const float& accuracyMod) :
    lineOfSight(sf::Lines, 2),
    lineTexture(),
    bullets(),
    MAX_DISTANCE_FIRED(maxDist),
    rotation(0.0),
    accuracyModifier(accuracyMod),
    queuedRotations(),
    fired(false),
    timeSinceFired(sf::seconds(0)),
    fireDelay(firingDelay),
    floor(OVERGROUND_FLOOR),
    bulletDamage(damage),
    maxCurrentMagazine(20),
    maxTotalMagazine(100),
    currentMagazine(maxCurrentMagazine),
    totalMagazine(maxTotalMagazine),
    reloading(false),
    animationTimer(),
    frameTime(sf::milliseconds(60)),
    frame(0),
    reloadingFrameCount(7)
    {
        //set a default location for the line of sight
        updateLineOfSight(sf::Vector2f(0, 0));
        lineOfSight[0].texCoords = sf::Vector2f(0, 0);
        lineOfSight[1].texCoords = sf::Vector2f(100, 0);

        lineTexture.loadFromFile("texture.png");
    }

void Gun::handleButtonPress() {

    //being firing and set the time since last fired equal to the fire delay that way gun fires immediately
    //so spamming the fire button is faster than holding it if user presses fast enough
    fired = true;
}

void Gun::handleButtonRelease() {

    fired = false;
}

void Gun::updateGunfire(const sf::Time& delta) {

    timeSinceFired += delta;

    //if gun if firing and the delay is over shoot a bullet
    if(canFireGun()) {

        fire();
        timeSinceFired = sf::seconds(0);

    }
}

void Gun::updateBullets(const sf::Time& delta) {

    //update all bullets and remove the ones that are dead
    for(unsigned i = 0; i < bullets.size();) {

        bullets[i]->updateElapsedTime(delta);

        //if a bullet can't be drawn it's dead and should be deleted
        if(!bullets[i]->canDraw()) {

            bullets.erase(bullets.begin() + i);
            continue;
        }

        i++;
    }
}

void Gun::updateRotation(const sf::Vector2f& playerPosition, const float& playerRotation) {

    rotation = playerRotation;
    updateLineOfSight(playerPosition);
}

bool Gun::reload() {

    if(canReload()) {

        reloading = true;
        resetAnimation();

        return true;
    }

    return false;
}

void Gun::animate() {

    //check if the number of frames exceeds the frame count for whatever animation is playing
    if(reloading) {

        //increase the frame count if needed
        if(animationTimer.getElapsedTime() > frameTime) {

            frame++;
            animationTimer.restart();
        }

        if(frame > reloadingFrameCount) {

            finishReloading();
            resetAnimation();
        }
    }
}

void Gun::drawAll(sf::RenderWindow& window) {

    drawSight(window);
    drawBullets(window);
}

void Gun::drawSight(sf::RenderWindow& window) {

    window.draw(lineOfSight, &lineTexture);
}

void Gun::drawBullets(sf::RenderWindow& window) {

    for(auto& bullet : bullets) {

        //only draw the bullet if it has checked for collision
        //if it hasn't hcecked for collision then it is a brand new bullet, and new bullets may look as if they are going through all other entities
        //when infact they check for collision the next update and onlly are cut off
        if(!bullet->checkCanCollide()) {

            bullet->draw(window);
        }
    }
}

vector<shared_ptr<Bullet> > Gun::getBullets() {

    return bullets;
}

const vector<float>& Gun::getQueuedRotations() const {

    return queuedRotations;
}

void Gun::clearRotations() {

    queuedRotations.clear();
}

void Gun::setFloor(const unsigned& newFloor) {

    floor = newFloor;
}

void Gun::setCurrentAmmo(const int& amount) {

    if(amount >= 0 && amount < maxCurrentMagazine) {

        currentMagazine = amount;
    }
}

const int Gun::getCurrentAmmo() const {

    return currentMagazine;
}

void Gun::setTotalAmmo(const int& amount) {

    if(amount > 0 && amount < maxTotalMagazine) {

        totalMagazine = amount;
    }
}

const int Gun::getTotalAmmo() const {

    return totalMagazine;
}

float Gun::fire() {

    //angle at which gun was fired
    float gunfireAngle = rotation;

    //modify the rotation by randomly adding and subtracting the accuracy modifier in order to randomly change range
    //because the range is always positive you have to subtract and add a value manually
    //add a minimum that isn't 0 that way it decreases the chances of hitting right in the middle
    gunfireAngle -= getAccuracyModifier();
    gunfireAngle += getRand(getAccuracyModifier() * 2, 0);

    sf::Vector2f bulletEndPoint = calculateEndPoint(lineOfSight[0].position, gunfireAngle);

    createBullet(lineOfSight[0].position, bulletEndPoint, floor);

    //now queue the rotation so data can be sent to server later
    queuedRotations.push_back(gunfireAngle);

    useAmmo();

    return gunfireAngle;
}

void Gun::fire(const float& angle) {

    //if there are no bullets then don't try to fire
    if(mustReload()) {

        return;
    }

    //calculate the end point of the bullet on the given angle
    sf::Vector2f endPoint = calculateEndPoint(lineOfSight[0].position, angle);

    //fire with new angle
    createBullet(lineOfSight[0].position, endPoint, floor);

    useAmmo();
}

void Gun::fire(const sf::Vector2f& bulletBegin, const sf::Vector2f& bulletEnd, const unsigned& bulletFloor) {

    createBullet(bulletBegin, bulletEnd, bulletFloor);

    //a bullet created using start and end points are bullets sent from the server
    //meaning thye have already checked for collision
    //so indcate the bullet has checked for collision, this bullet is the last bullet added to the container
    bullets[bullets.size() - 1]->disableCollision();
}

void Gun::updateLineOfSight(const sf::Vector2f& origin) {

    lineOfSight[0].position = origin;

    //calculate the end point of the line of sight
    sf::Vector2f endPoint = calculateEndPoint(origin, rotation);

    lineOfSight[1].position = endPoint;
}

sf::Vector2f Gun::calculateEndPoint(const sf::Vector2f& beginPoint, const float& angle) const {

    //using the angle you can figure out the x and y components of the
    //triangle formed form the origin to the end point because you know the
    //lenght of the hypotenuse
    float xComponent = cos(degreesToRadians(angle)) * MAX_DISTANCE_FIRED;

    //in games y is positive towards the bottom and negative towards the top
    //so inverse the sign of the y component because the math calculations assumes
    //y is positive towards the top and negative towards the bottom
    float yComponent = -sin(degreesToRadians(angle)) * MAX_DISTANCE_FIRED;

    //find final position by addition the components to the initial position
    //because the x and y components are already negative or positive depending on which
    //way the player is aiming in order to reach that position
    sf::Vector2f endPoint(beginPoint.x + xComponent, beginPoint.y + yComponent);

    return endPoint;
}

void Gun::createBullet(const sf::Vector2f& bulletBegin, const sf::Vector2f& bulletEnd, const unsigned& bulletFloor) {

    //create a line for the bullet
    shared_ptr<LineSegment> bulletLine = createLine(bulletBegin, bulletEnd,
                                                    "", sf::Vector2f(0, 0), sf::Vector2f(0, 0));
    shared_ptr<Bullet> bullet(new Bullet(bulletLine, bulletFloor, bulletDamage));

    bullets.push_back(bullet);
}

const float Gun::getAccuracyModifier() const {

    return accuracyModifier;
}

bool Gun::canFireGun() const {

    return timeSinceFired > fireDelay && fired && !reloading && (currentMagazine > 0);
}

void Gun::finishReloading() {

    reloading = false;

    //calculate how much ammo is needed to refill current magazine
    int ammoRequired = maxCurrentMagazine - currentMagazine;

    //if there isn't enough ammo to refill the magazine completely then refill using whatever ammo is available
    if(ammoRequired > totalMagazine) {

        ammoRequired = totalMagazine;
    }

    currentMagazine += ammoRequired;
    totalMagazine -= ammoRequired;
}

void Gun::resetAnimation() {

    animationTimer.restart();
    frame = 0;
}

bool Gun::canReload() const {

    bool isMagazineFull = (currentMagazine == maxCurrentMagazine);
    bool isAmmoLeft = (totalMagazine > 0);

    return !reloading && !isMagazineFull && isAmmoLeft;
}

bool Gun::mustReload() const {

    return currentMagazine == 0;
}

void Gun::useAmmo() {

    setCurrentAmmo(getCurrentAmmo() - 1);
}
