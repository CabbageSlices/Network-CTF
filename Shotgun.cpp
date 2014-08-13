#include "Shotgun.h"

Shotgun::Shotgun() :
    Gun(13, 500, sf::milliseconds(200), 80),
    bulletsFired(100 / bulletDamage)
    {

    }

void Shotgun::updateGunfire(const sf::Time& delta) {

    timeSinceFired += delta;

    //if gun if firing and the delay is over shoot a bullet
    if(fired && timeSinceFired > fireDelay) {

        //shotgun fires multiple bullets so fire multiple times
        for(int shotsFired = 0; shotsFired < bulletsFired; ++shotsFired) {

            fire();
        }

        timeSinceFired = sf::seconds(0);
    }
}
