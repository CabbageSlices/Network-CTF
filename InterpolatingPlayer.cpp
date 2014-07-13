#include "InterpolatingPlayer.h"
#include "Gun.h"

InterpolatingPlayer::InterpolatingPlayer():
    PlayerBase()
    {

    }

void InterpolatingPlayer::setInterpolationRotation(const float& newRotation) {

    pastRotation = destinationRotation;
    destinationRotation = newRotation;
}

void InterpolatingPlayer::fireGun(const sf::Vector2f& bulletBegin, const sf::Vector2f& bulletEnd) {

    gun->fire(bulletBegin, bulletEnd);
}

void InterpolatingPlayer::drawGun(sf::RenderWindow& window) {

    gun->drawBullets(window);
}
