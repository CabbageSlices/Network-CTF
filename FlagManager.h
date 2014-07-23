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

        void holdFlag(unsigned short teamId);

        void dropFlag(unsigned short teamId);

        //handles collision between the given enity and flags
        std::map<unsigned short, std::tr1::shared_ptr<Flag> >& getFlags();

        //draw the flags
        void draw(sf::RenderWindow& window);
};

#endif // FLAGMANAGER_H_INCLUDED
