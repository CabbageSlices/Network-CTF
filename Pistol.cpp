#include "Pistol.h"

sf::SoundBuffer Pistol::fireBuffer;
sf::SoundBuffer Pistol::reloadBuffer;

Pistol::Pistol() :
    Gun(20, 600, sf::milliseconds(325), 1.5)
    {
        //load the texture for the ui
        uiTexture.loadFromFile("images/pistolUI.png");
        uiSprite.setTexture(uiTexture);
        uiSprite.setColor(sf::Color(255, 255, 255, 150));

        setupClips();

        if(fireBuffer.getDuration() == sf::seconds(0)) {

            fireBuffer.loadFromFile("sounds/pistolFire.wav");
            reloadBuffer.loadFromFile("sounds/pistolReload.wav");
        }

        fireSound.setBuffer(fireBuffer);
        reloadSound.setBuffer(reloadBuffer);

        animationTime = sf::milliseconds(80);
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
