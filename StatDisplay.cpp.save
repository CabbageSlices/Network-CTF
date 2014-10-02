#include "StatDisplay.h"

using std::string;

StatDisplay::StatDisplay(const sf::Vector2u& screenSize) :
    statDisplayTexture(),
    statDisplaySprite(),
    NAME_OFFSET(94, 24),
    HEALTH_OFFSET(104, 48),
    healthBar(91, 12),
    font(),
    nameScale(0.35),
    playerName()
    {
        statDisplayTexture.loadFromFile("statDisplay.png");
        statDisplaySprite.setTexture(statDisplayTexture);

        font.loadFromFile("font.ttf");

        playerName.setFont(font);
        playerName.setScale(nameScale, nameScale);
        playerName.setString("Player Name");

        handleScreenResize(screenSize);
    }

void StatDisplay::handleScreenResize(const sf::Vector2u& screenSize) {

    //always position the display at the bottom left position of the screen
    statDisplaySprite.setPosition(0, screenSize.y - statDisplaySprite.getGlobalBounds().height);

    setupComponentPositions();
}

void StatDisplay::setPlayerHealth(const float& currentValue) {

    healthBar.setCurrentHealth(currentValue);
}

void StatDisplay::setPlayerName(const string& name) {

    playerName.setString(name);
}

void StatDisplay::draw(sf::RenderWindow& window) {

    healthBar.draw(window);
    window.draw(statDisplaySprite);
    window.draw(playerName);
}

void StatDisplay::setupComponentPositions() {

    healthBar.setPosition(sf::Vector2f(statDisplaySprite.getPosition().x + HEALTH_OFFSET.x,
                          statDisplaySprite.getPosition().y + HEALTH_OFFSET.y));

    playerName.setPosition(sf::Vector2f(statDisplaySprite.getPosition().x + NAME_OFFSET.x,
                           statDisplaySprite.getPosition().y + NAME_OFFSET.y));
}
