#ifndef STATDISPLAY_H_INCLUDED
#define STATDISPLAY_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "Healthbar.h"

#include <string>
#include <vector>

//the player's name, ammo, health, and equipped weapon
class StatDisplay {

    private:

        //texture for the stat display outline
        sf::Texture statDisplayTexture;
        sf::Sprite statDisplaySprite;

        //current ammo is represented by pictures of bullets so have textures for the bullets
        //one picture to represent a used bullet slot, and one to represent an availabel bullet slot
        sf::Texture availableAmmoTexture;
        sf::Texture usedAmmoTexture;

        sf::Sprite availableAmmo;
        sf::Sprite usedAmmo;

        //distanec to the given object relative to the position of the stat display
        const sf::Vector2f NAME_OFFSET;
        const sf::Vector2f HEALTH_OFFSET;

        //current ammo is displayed as bullets, number of bullets = ammo
        //total ammo is represented as a number
        const sf::Vector2f AMMO_OFFSET_CURRENT;
        const sf::Vector2f AMMO_OFFSET_TOTAL;

        //player's healthbar, drawn underneath the statdisplay where the healthoutline is
        //do this because the health display has a custom shape and you cant get that using sfml shape
        //size of the healthbar is determined by the imagefile for the statdisplay
        HealthBar healthBar;

        //display for all texts displayed on the stat
        sf::Font font;
        const float TEXT_SCALE;

        sf::Text playerName;
        sf::Text totalAmmo;

        //make sure all of the player's information components (name, health, ammo) are in the correct position
        //relative to the position of the stat display
        void setupComponentPositions();

    public:

        StatDisplay(const sf::Vector2u& screenSize);

        void handleScreenResize(const sf::Vector2u& screenSize);

        void setPlayerHealth(const float& currentValue);
        void setPlayerName(const std::string& name);
        void setPlayerAmmo(const int currentAmmo, const int maxUsableAmmo, const int playerTotalAmmo);

        void draw(sf::RenderWindow& window);
};

#endif // STATDISPLAY_H_INCLUDED
