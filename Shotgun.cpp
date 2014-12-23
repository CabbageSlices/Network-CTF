#include "Shotgun.h"

#include <iostream>

using std::cout;
using std::endl;

Shotgun::Shotgun() :
    Gun(15, 400, sf::milliseconds(1000), 30),
    bulletsPerShot(5),
    bulletsFired(0)
    {
        originToLine = sf::Vector2f(43, 2);

        maxCurrentMagazine = 4;
        maxTotalMagazine = 12;
        currentMagazine = maxCurrentMagazine;
        totalMagazine = maxTotalMagazine;

        //load the texture for the ui
        uiTexture.loadFromFile("shotgunUI.png");
        uiSprite.setTexture(uiTexture);

        setupClips();
    }

void Shotgun::updateGunfire(const sf::Time& delta) {

    timeSinceFired += delta;

    //if gun if firing and the delay is over shoot a bullet
    if(canFireGun()) {

        //shotgun fires multiple bullets so fire multiple times
        for(int shotsFired = 0; shotsFired < bulletsPerShot; ++shotsFired) {

            fire();
        }

        timeSinceFired = sf::seconds(0);
    }
}

GunTypes Shotgun::getGunType() {

    return SHOTGUN;
}

void Shotgun::setupClips() {

    //height on the image where the animations for this gun begins
    float animationHeight = 144;

    standingClips.push_back(sf::IntRect(3, animationHeight, 59, 43));

    reloadingClips.push_back(sf::IntRect(318, animationHeight, 59, 43));
    reloadingClips.push_back(sf::IntRect(381, animationHeight, 59, 43));
    reloadingClips.push_back(sf::IntRect(444, animationHeight, 59, 43));

    shootingClips.push_back(sf::IntRect(66, animationHeight, 59, 43));
    shootingClips.push_back(sf::IntRect(129, animationHeight, 59, 43));
    shootingClips.push_back(sf::IntRect(192, animationHeight, 59, 43));
    shootingClips.push_back(sf::IntRect(255, animationHeight, 59, 43));

}

void Shotgun::useAmmo() {

    //every time a bullet is fired increase the bullets fired count
    //when the gun has fired as many bullets as the amount of bullets that should be fired per shot
    //decurease ammo count
    bulletsFired++;

    if(bulletsFired >= bulletsPerShot) {

        bulletsFired = 0;
        Gun::useAmmo();
    }
}
