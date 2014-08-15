#include "Shotgun.h"

#include <iostream>

using std::cout;
using std::endl;

Shotgun::Shotgun() :
    Gun(13, 500, sf::milliseconds(100), 30),
    bulletsPerShot(100 / bulletDamage),
    bulletsFired(0)
    {
        maxCurrentMagazine = 4;
        maxTotalMagazine = 12;
        currentMagazine = maxCurrentMagazine;
        totalMagazine = maxTotalMagazine;
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

GunTypes Shotgun::getGunType() const {

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
