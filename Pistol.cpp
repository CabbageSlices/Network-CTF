#include "Pistol.h"

Pistol::Pistol() :
    Gun(15, 1200.f, sf::milliseconds(550), 4)
    {
        //load the texture for the ui
        uiTexture.loadFromFile("pistolUI.png");
        uiSprite.setTexture(uiTexture);
        uiSprite.setColor(sf::Color(255, 255, 255, 150));

        setupClips();
    }

void Pistol::setupClips() {

    //height on the image where the animations for this gun begins
    float animationHeight = 191;

    standingClips.push_back(sf::IntRect(3, animationHeight, 59, 43));

    reloadingClips.push_back(sf::IntRect(318, animationHeight, 59, 43));
    reloadingClips.push_back(sf::IntRect(381, animationHeight, 59, 43));
    reloadingClips.push_back(sf::IntRect(444, animationHeight, 59, 43));

    shootingClips.push_back(sf::IntRect(66, animationHeight, 59, 43));
    shootingClips.push_back(sf::IntRect(129, animationHeight, 59, 43));
    shootingClips.push_back(sf::IntRect(192, animationHeight, 59, 43));
}
