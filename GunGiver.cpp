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

sf::Texture GunGiver::messageTextures;

map<GunTypes, sf::Sprite> GunGiver::messageSprites;

GunGiver::GunGiver(const GunTypes& gun) :
    StaticObject(sf::Vector2f(0, 0)),
    gunToGive(gun),
    shouldDrawMessage(false)
    {
        //if the textures for the gun givers aren't created yet, then create them
        if(!texturesCreated) {

            initialize();
        }

        //textures are created so set the texture for this collision box
        collisionBox.setTexture(&textures[gun]);
        collisionBox.setSize(sf::Vector2f(collisionBox.getTexture()->getSize().x, collisionBox.getTexture()->getSize().y));
        collisionBox.setOrigin(calculateCenter(collisionBox.getGlobalBounds()));
    }

void GunGiver::initialize() {

    textures[SHOTGUN] = sf::Texture();
    textures[SHOTGUN].loadFromFile("images/shotgunIcon.png");

    textures[PISTOL] = sf::Texture();
    textures[PISTOL].loadFromFile("images/pistolIcon.png");

    textures[SNIPER] = sf::Texture();
    textures[SNIPER].loadFromFile("images/sniperIcon.png");

    messageTextures.loadFromFile("images/gameText.png");

    //create sprites for each gun type
    messageSprites[SHOTGUN] = sf::Sprite();
    messageSprites[SHOTGUN].setTexture(messageTextures);
    messageSprites[SHOTGUN].setTextureRect(sf::IntRect(1, 129, 382, 31));

    messageSprites[PISTOL] = sf::Sprite();
    messageSprites[PISTOL].setTexture(messageTextures);
    messageSprites[PISTOL].setTextureRect(sf::IntRect(1, 161, 382, 31));

    messageSprites[SNIPER] = sf::Sprite();
    messageSprites[SNIPER].setTexture(messageTextures);
    messageSprites[SNIPER].setTextureRect(sf::IntRect(1, 193, 382, 31));

    //position all sprites now
    for(auto& pair: messageSprites) {

        sf::Sprite& sprite = pair.second;

        //window size is always 1024 by 768 so just center each sprite at the very bottom of the window
        sprite.setPosition(1024 / 2 - sprite.getGlobalBounds().width / 2, 768 - sprite.getGlobalBounds().height);
    }

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

    } else if(isType(collidingPlayer.getGun(), gunToGive)) {

        //same guns so refil ammo
        collidingPlayer.getGun()->refillTotalAmmo();
    }
}
