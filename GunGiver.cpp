#include "GunGiver.h"
#include "PlayerBase.h"
#include "GunHandlers.h"
#include "Gun.h"
#include "math.h"

#include <iostream>

using std::cout;
using std::endl;
using std::map;

map<GunTypes, sf::Texture> GunGiver::textures;
bool GunGiver::texturesCreated = false;

GunGiver::GunGiver(const GunTypes& gun) :
    StaticObject(sf::Vector2f(0, 0)),
    gunToGive(gun)
    {
        //if the textures for the gun givers aren't created yet, then create them
        if(!texturesCreated) {

            initializeTextures();
        }

        //textures are created so set the texture for this collision box
        collisionBox.setTexture(&textures[gun]);
        collisionBox.setSize(sf::Vector2f(collisionBox.getTexture()->getSize().x, collisionBox.getTexture()->getSize().y));
        collisionBox.setOrigin(calculateCenter(collisionBox.getGlobalBounds()));
    }

void GunGiver::initializeTextures() {

    textures[SHOTGUN] = sf::Texture();
    textures[SHOTGUN].loadFromFile("shotgunIcon.png");

    textures[PISTOL] = sf::Texture();
    textures[PISTOL].loadFromFile("pistolIcon.png");

    textures[SNIPER] = sf::Texture();
    textures[SNIPER].loadFromFile("sniperIcon.png");

    texturesCreated = true;
}

GunTypes GunGiver::getGunType() const {

    return gunToGive;
}

void GunGiver::setPosition(const sf::Vector2f& position) {

    collisionBox.setPosition(position);
}

void GunGiver::handleCollision(PlayerBase& collidingPlayer) {

    //if player doesn't have this gun, give him this gun, otherwise reload his ammo
    //only give gun if hes picking up gun
    if(!isType(collidingPlayer.getGun(), gunToGive) && collidingPlayer.canPickUpGun()) {

        collidingPlayer.setGun(createGun(gunToGive));

    } else {

        //same guns so refil ammo
        collidingPlayer.getGun()->refillTotalAmmo();
    }
}
