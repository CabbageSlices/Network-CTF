#include "StatDisplay.h"
#include "Conversion.h"
#include "Gun.h"

using std::string;
using std::tr1::shared_ptr;

StatDisplay::StatDisplay(const sf::Vector2u& screenSize) :
    statDisplayTexture(),
    statDisplaySprite(),
    availableAmmoTexture(),
    usedAmmoTexture(),
    availableAmmo(),
    usedAmmo(),
    NAME_OFFSET(94, 24),
    HEALTH_OFFSET(104, 48),
    AMMO_OFFSET_CURRENT(103, 61),
    AMMO_OFFSET_TOTAL(184, 61),
    healthBar(91, 12),
    font(),
    TEXT_SCALE(0.35),
    playerName(),
    totalAmmo()
    {
        statDisplayTexture.loadFromFile("statDisplay.png");
        statDisplaySprite.setTexture(statDisplayTexture);

        availableAmmoTexture.loadFromFile("availableAmmo.png");
        availableAmmoTexture.setRepeated(true);
        availableAmmo.setTexture(availableAmmoTexture);

        usedAmmoTexture.loadFromFile("usedAmmo.png");
        usedAmmoTexture.setRepeated(true);
        usedAmmo.setTexture(usedAmmoTexture);

        font.loadFromFile("font.ttf");

        playerName.setFont(font);
        playerName.setScale(TEXT_SCALE, TEXT_SCALE);
        playerName.setString("Player Name");

        totalAmmo.setFont(font);
        totalAmmo.setScale(TEXT_SCALE, TEXT_SCALE);
        totalAmmo.setString("0");
        totalAmmo.setColor(sf::Color(66, 40, 57));

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

void StatDisplay::setPlayerAmmo(const int currentAmmo, const int maxUsableAmmo, const int playerTotalAmmo) {

    availableAmmo.setTextureRect(sf::IntRect(0, 0, availableAmmoTexture.getSize().x * currentAmmo,
                                               availableAmmoTexture.getSize().y));

    usedAmmo.setTextureRect(sf::IntRect(0, 0, usedAmmoTexture.getSize().x * maxUsableAmmo,
                                          usedAmmoTexture.getSize().y));

    totalAmmo.setString(toString(playerTotalAmmo));
}

void StatDisplay::draw(sf::RenderWindow& window) {

    healthBar.draw(window);
    window.draw(statDisplaySprite);
    window.draw(playerName);

    window.draw(usedAmmo);
    window.draw(availableAmmo);
    window.draw(totalAmmo);
}

void StatDisplay::drawGunUI(sf::RenderWindow& window, shared_ptr<Gun> playerGun) {

    //the offsets from the top left of the statDisplay is based on the image file, so use that
    sf::Vector2f gunOffset(8, 0);

    playerGun->drawUI(gunOffset + statDisplaySprite.getPosition(), window);
}

void StatDisplay::setupComponentPositions() {

    healthBar.setPosition(sf::Vector2f(statDisplaySprite.getPosition().x + HEALTH_OFFSET.x,
                          statDisplaySprite.getPosition().y + HEALTH_OFFSET.y));

    playerName.setPosition(sf::Vector2f(statDisplaySprite.getPosition().x + NAME_OFFSET.x,
                           statDisplaySprite.getPosition().y + NAME_OFFSET.y));

    availableAmmo.setPosition(sf::Vector2f(statDisplaySprite.getPosition().x + AMMO_OFFSET_CURRENT.x,
                                           statDisplaySprite.getPosition().y + AMMO_OFFSET_CURRENT.y));

    usedAmmo.setPosition(sf::Vector2f(statDisplaySprite.getPosition().x + AMMO_OFFSET_CURRENT.x,
                                      statDisplaySprite.getPosition().y + AMMO_OFFSET_CURRENT.y));

    totalAmmo.setPosition(sf::Vector2f(statDisplaySprite.getPosition().x + AMMO_OFFSET_TOTAL.x,
                                       statDisplaySprite.getPosition().y + AMMO_OFFSET_TOTAL.y));
}
