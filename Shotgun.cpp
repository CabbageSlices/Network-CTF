#include "Shotgun.h"

#include <iostream>

using std::cout;
using std::endl;

Shotgun::Shotgun() :
    Gun(15, 400, sf::milliseconds(1000), 30),
    bulletsPerShot(5),
    bulletsFired(0)
    {
        maxCurrentMagazine = 4;
        maxTotalMagazine = 12;
        currentMagazine = maxCurrentMagazine;
        totalMagazine = maxTotalMagazine;

        //load the texture for the ui
        uiTexture.loadFromFile("shotgunUI.png");
        uiSprite.setTexture(uiTexture);
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
