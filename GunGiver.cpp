#include "GunGiver.h"
#include "PlayerBase.h"
#include "GunHandlers.h"
#include "Gun.h"

#include <iostream>

using std::cout;
using std::endl;

GunGiver::GunGiver(const sf::Vector2f& centerPosition, const GunTypes& gun) :
    StaticObject(centerPosition),
    gunToGive(gun)
    {
        collisionBox.setFillColor(sf::Color(124, 215, 221, 200));
    }

void GunGiver::handleCollision(PlayerBase& collidingPlayer) {

    //if player doesn't have this gun, give him this gun, otherwise reload his ammo
    if(!isType(collidingPlayer.getGun(), gunToGive)) {

        collidingPlayer.setGun(createGun(gunToGive));

    } else {

        //same guns so refil ammo
        collidingPlayer.getGun()->refillTotalAmmo();
    }
}
