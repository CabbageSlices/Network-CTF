#ifndef PLAYERBASE_H_INCLUDED
#define PLAYERBASE_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "Healthbar.h"

#include <vector>
#include <tr1/memory>

class Gun;
class Bullet;


class PlayerBase {

    protected:

        //id of this player, supplied by the server
        int playerId;

        //id of the team the player is in
        unsigned short teamId;

        //players hit box for drawing, movement, and collision
        //current hitbox is the current position of the player which will interpolate towards the destination box
        //previous hitbox is the past position of the player which interpolates towards the destination box
        //past and destination are used for interpolation, past is the beginning point of interpolation
        sf::RectangleShape pastHitBox;
        sf::RectangleShape currentHitBox;
        sf::RectangleShape destinationHitBox;

        HealthBar health;

        //image for players sprite
        sf::Texture playerTexture;

        sf::Sprite playerSprite;

        //rotation of the player in degrees, interolates like the hitbox
        float pastRotation;
        float currentRotation;
        float destinationRotation;

        std::tr1::shared_ptr<Gun> gun;

        bool holdingFlag;

        void updateHitboxRotation();

        //update teh position of the healthbar, tracks players current hitbox
        void updateHealthPosition();

        //update the position of the player's sprite, places it at the players hitbox position
        void updateSpritePosition();

        //overload in inherited class to draw differnt part of guns
        virtual void drawGun(sf::RenderWindow& window) = 0;

    public:

        PlayerBase();

        std::vector<std::tr1::shared_ptr<Bullet> > getBullets();

        int getId();
        void setId(const int& id);

        unsigned short getTeam() const;
        void setTeam(const unsigned short& team);

        const float getRotation() const;

        void updateGun(const float& delta);

        //make player interpolate from current position to the given position
        //called by several functions related to the player class and interpolationg player class
        void setInterpolationPosition(const sf::Vector2f& position);

        //interpolate from previous position to the destination position
        ///time passed is what fraction of time has passed from the physics update to the next physics update, not the actual time passed
        void interpolate(const float& deltaFraction);
        void draw(sf::RenderWindow& window);

        const sf::Vector2f& getDestinationPosition() const;

        const sf::Vector2f& getCurrentPosition() const;

        //just returns the player's current position box, usually jsut used to indicate his properties
        sf::FloatRect getCurrentHitbox() const;

        sf::FloatRect getCollisionBox() const;

        sf::FloatRect getDestinationBox() const;

        void move(const sf::Vector2f& offset);

        int getHealth() const;

        //uses sethealth
        void getHit(int damage);

        //set helath to given value, if its less than current health then run damage animation
        void setHealth(int value);

        bool isHoldingFlag() const;

        void holdFlag();

        void dropFlag();

        void setHoldingFlag(const bool& idHolding);
};

#endif // PLAYERBASE_H_INCLUDED
