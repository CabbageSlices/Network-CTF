#include "InterpolatingPlayer.h"
#include "Gun.h"
#include "math.h"

InterpolatingPlayer::InterpolatingPlayer():
    PlayerBase(),
    nameText(),
    font(),
    shouldDraw(true)
    {
        font.loadFromFile("font.ttf");
        nameText.setFont(font);
        nameText.setScale(0.35, 0.35);
        nameText.setString(playerName);
        nameText.setColor(sf::Color::Black);
    }

void InterpolatingPlayer::setInterpolationRotation(const float& newRotation) {

    pastRotation = destinationRotation;
    destinationRotation = newRotation;
}

void InterpolatingPlayer::fireGun(const sf::Vector2f& bulletBegin, const sf::Vector2f& bulletEnd, const unsigned& floor) {

    gun->fire(bulletBegin, bulletEnd, floor);
}

void InterpolatingPlayer::drawGun(sf::RenderWindow& window, const unsigned& drawingFloor) {

    gun->drawGun(window);
    gun->drawBullets(window, drawingFloor);
}

PlayerBase::DrawingState InterpolatingPlayer::getDrawingState() {

    if(compareFloatRects(destinationHitBox.getGlobalBounds(), pastHitBox.getGlobalBounds()) && health.getCurrentHealth() > 0 && !playSpawnAnimation) {

        return STANDING;
    }

    if(health.getCurrentHealth() == 0) {

        return DYING;
    }

    if(playSpawnAnimation) {

        return SPAWNING;
    }

    return WALKING;
}
