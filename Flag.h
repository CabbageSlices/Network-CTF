#ifndef FLAG_H_INCLUDED
#define FLAG_H_INCLUDED

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

class Flag {

    private:

        //flag image to draw
        sf::CircleShape flag;

        //position of the spawn
        sf::Vector2f spawnPosition;

        //if flag is being held no other players may grab it
        bool beingHeld;

        //if the flag is at the spawn
        bool atSpawn;

    public:

        Flag(const sf::Vector2f& spawnLocation, const sf::Color& flagColor = sf::Color::Red);

        void draw(sf::RenderWindow& window);

        void setPosition(const sf::Vector2f& position);
        void reset();

        void dropFlag();
        void pickUpFlag();

        const bool& isHeld() const;
        const bool& isAtSpawn() const;

        const sf::FloatRect getCollisionBox() const;

        void setHeld(bool held);
};

#endif // FLAG_H_INCLUDED