#include "Sniper.h"

Sniper::Sniper() :
    Gun(75, 2500, sf::milliseconds(1000), 15),
    holdingFireButton(false),
    currentAccuracyModifier(accuracyModifier),
    accuracyChangeRate(6),
    accuracyRange(sf::Triangles, 3)
    {
        maxCurrentMagazine = 2;
        maxTotalMagazine = 10;
        currentMagazine = maxCurrentMagazine;
        totalMagazine = maxTotalMagazine;

        setupAccuracyRange();

        //load the texture for the ui
        uiTexture.loadFromFile("sniperUI.png");
        uiSprite.setTexture(uiTexture);
    }

void Sniper::handleButtonPress() {

    holdingFireButton = true;
}

void Sniper::handleButtonRelease() {

    fired = true;
    holdingFireButton = false;
}

void Sniper::updateGunfire(const sf::Time& delta) {

    timeSinceFired += delta;

    //if the fire delay has been passed begin preparing to fire, that is
    //begin to increase accuracy if player is holding down fire button

    if(canIncreaseAccuracy()) {

        currentAccuracyModifier -= accuracyChangeRate * delta.asSeconds();
        currentAccuracyModifier = currentAccuracyModifier < 0 ? 0 : currentAccuracyModifier;
    }

    if(canFireGun()) {

        fire();

        //gun was fired so reset the state
        fired = false;
        timeSinceFired = sf::seconds(0);

        //reset accuracy modifier so gun has to aim again
        currentAccuracyModifier = accuracyModifier;

    } else {

        //if he can't fire gun but he has fired the gun then the fired won't change to false
        //and the gun will automatically fire later on by itself so change fired to false
        //so gun wont fire by itself
        fired = false;
    }
}

void Sniper::drawSight(sf::RenderWindow& window) {

    window.draw(accuracyRange);
    Gun::drawSight(window);
}

GunTypes Sniper::getGunType() {

    return SNIPER;
}

bool Sniper::canIncreaseAccuracy() {

    return timeSinceFired > fireDelay && holdingFireButton && !mustReload();
}

void Sniper::updateLineOfSight(const sf::Vector2f& origin) {

    //first update the normal line of sight
    Gun::updateLineOfSight(origin);

    //now update the accuracy range
    setupAccuracyPoints();
}

void Sniper::setupAccuracyRange() {

    setupAccuracyPoints();
    setupAccuracyColour();
}

void Sniper::setupAccuracyPoints() {

    //calculate the three points used to draw the triangle that forms the accuracy range
    //first point is the origin of the gun, which is the origin of the line of sight
    sf::Vector2f point1 = lineOfSight[0].position;

    //end points of the triangle are the end points of the line that forms from the origin of the gun
    //to the max distance fired at the lowest range of the accuracy and the highest range
    sf::Vector2f point2 = calculateEndPoint(point1, rotation - currentAccuracyModifier);
    sf::Vector2f point3 = calculateEndPoint(point1, rotation + currentAccuracyModifier);

    accuracyRange[0].position = point1;
    accuracyRange[1].position = point2;
    accuracyRange[2].position = point3;
}

void Sniper::setupAccuracyColour() {

    accuracyRange[0].color = sf::Color(50, 50, 50);
    accuracyRange[1].color = sf::Color::Transparent;
    accuracyRange[2].color = sf::Color::Transparent;
}

const float Sniper::getAccuracyModifier() const {

    return currentAccuracyModifier;
}
