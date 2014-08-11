#ifndef PORTAL_H_INCLUDED
#define PORTAL_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "StaticObject.h"
#include "Floors.h"

//when a player collides with a portal they are taken to a different part of the map or a different floor entirely
//used to have caves and other stuff
class Portal : public StaticObject {

    private:

        //location to teleport to once user hits the portal
        //also servers as a drawing position
        sf::RectangleShape teleportPosition;

        //floor the portal takes the player to
        unsigned destinationFloor;

    public:

        Portal(const sf::Vector2f& centerPosition, const sf::Vector2f& teleportLocation, const unsigned destinationFloor = UNDERGROUND_FLOOR);
        Portal(const sf::Vector2f& centerPosition);

        virtual ~Portal(){/*empty destructor*/}

        void setTeleportPosition(const sf::Vector2f& position, const unsigned destinationFloor = UNDERGROUND_FLOOR);
        const sf::Vector2f& getTeleportPosition() const;

        const unsigned getDestinationFloor() const;

        virtual void draw(sf::RenderWindow& window);
};

#endif // PORTAL_H_INCLUDED
