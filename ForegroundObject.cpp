#include "ForegroundObject.h"

ForegroundObject::ForegroundObject(const sf::Vector2f& centerPosition) :
    StaticObject(centerPosition),
    hidingPlayer(false)
    {
        setupCollisionBox();
    }

void ForegroundObject::setHidingPlayer(const bool& playerHidden) {

    hidingPlayer = playerHidden;

    //make the object see through so player can see whats behind it when he is hiding
    if(hidingPlayer) {

    makeSeethrough();

    } else {

     makeSolid();
    }
}

void ForegroundObject::setupCollisionBox() {

    collisionBox.setSize(sf::Vector2f(100, 100));
}

void ForegroundObject::makeSeethrough() {

    collisionBox.setFillColor(sf::Color(255, 255, 255, 150));
}

void ForegroundObject::makeSolid() {

    collisionBox.setFillColor(sf::Color(255, 255, 255, 255));
}
