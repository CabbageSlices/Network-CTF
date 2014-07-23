#ifndef FLAGMANAGER_H_INCLUDED
#define FLAGMANAGER_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

#include <tr1/memory>
#include <map>

//keeps track of each teams flags and their flag spawn zone
class FlagManager {

    private:

        struct Flag {

            //flag image to draw
            sf::CircleShape flag;

            //position of the spawn
            sf::Vector2f spawnPosition;
        };

        //container to hold flags
        //the map's key is the id of the team the flag is associated with
        //ex - team 1 has an id of 1 so their flag has a key of 1 in the map
        std::map<unsigned short, std::tr1::shared_ptr<Flag> > flags;

        void addFlag(const sf::Vector2f& flagSpawnLocation, unsigned short teamId);

    public:

        FlagManager(const sf::Vector2f& teamASpawn, const sf::Vector2f& teamBSpawn);

        void setFlagPosition(const sf::Vector2f& flagPosition, unsigned short teamId);

        //draw the flags
        void draw(sf::RenderWindow& window);
};

#endif // FLAGMANAGER_H_INCLUDED
