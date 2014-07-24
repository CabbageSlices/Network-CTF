#ifndef FLAGMANAGER_H_INCLUDED
#define FLAGMANAGER_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

#include <tr1/memory>
#include <map>

//keeps track of each teams flags and their flag spawn zone
class FlagManager {

    public:

        struct Flag {

            //flag image to draw
            sf::CircleShape flag;

            //position of the spawn
            sf::Vector2f spawnPosition;

            //if flag is being held no other players may grab it
            bool beingHeld = false;

            //if the flag is at the spawn
            bool atSpawn = true;
        };

    private:

        //container to hold flags
        //the map's key is the id of the team the flag is associated with
        //ex - team 1 has an id of 1 so their flag has a key of 1 in the map
        std::map<unsigned short, std::tr1::shared_ptr<Flag> > flags;

        void addFlag(const sf::Vector2f& flagSpawnLocation, unsigned short teamId);

    public:

        FlagManager(const sf::Vector2f& teamASpawn, const sf::Vector2f& teamBSpawn);

        void setFlagPosition(const sf::Vector2f& flagPosition, unsigned short teamId);

        //check if the flag of the given team is taken
        bool canHoldFlag(unsigned short teamId);

        //check if the given flag is at its spawn location
        bool flagAtSpawn(unsigned short teamId);

        void holdFlag(unsigned short teamId);

        void dropFlag(unsigned short teamId);

        //return all flags back to spawn locations
        void resetFlags();

        //check for collision between the given entity and the given flag
        bool checkFlagCollision(const sf::FloatRect& collisionBox, const unsigned short& teamId);

        //return the given flag back to the base
        void flagToSpawn(const unsigned short& teamId);

        //draw the flags
        void draw(sf::RenderWindow& window);
};

#endif // FLAGMANAGER_H_INCLUDED
