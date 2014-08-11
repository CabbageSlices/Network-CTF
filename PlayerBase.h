#ifndef PLAYERBASE_H_INCLUDED
#define PLAYERBASE_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "Healthbar.h"
#include "FlagManager.h"

#include <vector>
#include <tr1/memory>

class Gun;
class Bullet;
class Flag;

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

        //flag the player is holding
        std::tr1::weak_ptr<Flag> flagBeingHeld;

        //timer to time how long its been since the last time data about this player was received from either the server or client
        sf::Clock dataReceiveTimer;

        //maximum amount of time this player should go without receiving data
        sf::Time maxNoData;

        //timer to respawn player after he is dead
        sf::Clock respawnTimer;
        sf::Time respawnDelay;

        unsigned short kills;
        unsigned short deaths;
        unsigned short flagCaptures;
        unsigned short flagReturns;

        //maximum distance the player can interpolate to, this is the squared distnace
        const float maxInterpolationDist;

        //current floor of the player, can only interact with others in the same floor
        unsigned short currentFloor;

        //sets position without interpolation
        void setPosition(const sf::Vector2f& position);

        void updateHitboxRotation();

        //update teh position of the healthbar, tracks players current hitbox
        void updateHealthPosition();

        //update the position of the player's sprite, places it at the players hitbox position
        void updateSpritePosition();

        void die();

        //overload in inherited class to draw differnt part of guns
        virtual void drawGun(sf::RenderWindow& window) = 0;

    public:

        PlayerBase();

        ~PlayerBase();

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
        void stopInterpolation();

        void draw(sf::RenderWindow& window);

        //drawing components to show on minimap
        void drawMinimap(sf::RenderWindow& window);

        const sf::Vector2f& getDestinationPosition() const;

        const sf::Vector2f& getCurrentPosition() const;

        //just returns the player's current position box, usually jsut used to indicate his properties
        sf::FloatRect getCurrentHitbox() const;

        sf::FloatRect getCollisionBox() const;

        sf::FloatRect getDestinationBox() const;

        void move(const sf::Vector2f& offset);

        int getHealth() const;

        bool isAlive() const;
        bool shouldRespawn() const;

        void respawn(const sf::Vector2f& spawnPosition);

        //uses sethealth
        void getHit(int damage);

        //set helath to given value, if its less than current health then run damage animation
        void setHealth(int value);

        bool isHoldingFlag() const;

        void holdFlag(std::tr1::shared_ptr<Flag> flagToHold);

        void dropFlag();

        //drop the flag at the given position
        void dropFlag(const sf::Vector2f& position);

        void setFloor(const unsigned& destinationFloor);
        const unsigned getFloor() const;

        bool timedOut();
        void resetDataTimer();

        const unsigned short& getKills() const;
        const unsigned short& getDeaths() const;
        const unsigned short& getFlagCaptures() const;
        const unsigned short& getFlagReturns() const;

        void setKills(const unsigned short& amount);
        void setDeaths(const unsigned short& amount);
        void setCaptures(const unsigned short& amount);
        void setReturns(const unsigned short& amount);
};

#endif // PLAYERBASE_H_INCLUDED
