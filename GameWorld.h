#ifndef GAMEWORLD_H_INCLUDED
#define GAMEWORLD_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "Floors.h"
#include "LargeImage.h"
#include "Floor.h"

#include <map>
#include <vector>
#include <tr1/memory>
#include <string>

class Block;
class ForegroundObject;
class FlagManager;
class PlayerBase;
class Portal;
class GunGiver;

//class to manage the game world by keeping track of all of the objects and events
class GameWorld {

    private:

        //there are too many collision boxes for objects in the game world to collide with
        //so divide the world into smaller peices and only allow objects to check for collision with other objects within the same section
        //regions are just how many sections the floor is divided into
        struct Regions {

            //preset number of sections to divide the floor into
            std::vector<std::tr1::shared_ptr<Block> > blocksSection1;
            std::vector<std::tr1::shared_ptr<Block> > blocksSection2;
            std::vector<std::tr1::shared_ptr<Block> > blocksSection3;
            std::vector<std::tr1::shared_ptr<Block> > blocksSection4;
            std::vector<std::tr1::shared_ptr<Block> > blocksSection5;
            std::vector<std::tr1::shared_ptr<Block> > blocksSection6;
            std::vector<std::tr1::shared_ptr<Block> > blocksSection7;
            std::vector<std::tr1::shared_ptr<Block> > blocksSection8;
        };

        const int SECTIONS_IN_FLOOR;

        std::map<const unsigned, std::tr1::shared_ptr<Floor> > floors;
        std::map<const unsigned, Regions> regions;

        std::tr1::shared_ptr<FlagManager> flagManager;

        //spawn zones of each team
        sf::FloatRect teamASpawn;
        sf::FloatRect teamBSpawn;

        void createRegions();

        //returns the specific section of the region that the given point exists in
        std::vector<std::tr1::shared_ptr<Block> >& getContainingSection(const sf::Vector2f& point, const unsigned& floor);

    public:

        GameWorld();

        //returns all blocks on the given floor
        std::vector<std::tr1::shared_ptr<Block> >& getBlocks(const unsigned& floor = OVERGROUND_FLOOR);

        //finds out which region the given two poinst encompass, that is it checks the line that forms from the beginning point to the ending point
        //and checks which regions the line touches, and returns all blocks within the regions, the given container has no duplicates
        std::vector<std::tr1::shared_ptr<Block> > getBlocks(const sf::Vector2f& beginRange, const sf::Vector2f& endRange, const unsigned& floor = OVERGROUND_FLOOR);

        std::vector<std::tr1::shared_ptr<ForegroundObject> >& getForeground(const unsigned& floor = OVERGROUND_FLOOR);
        std::vector<std::tr1::shared_ptr<Portal> >& getPortals(const unsigned& floor = OVERGROUND_FLOOR);
        std::vector<std::tr1::shared_ptr<GunGiver> >& getGunGivers(const unsigned& floor = OVERGROUND_FLOOR);

        std::tr1::shared_ptr<FlagManager> getFlagManager();

        const sf::FloatRect& getSpawnArea(const unsigned& team) const;

        //get a spawn position for the given team, randomly gets a point wihtin that team's spawn zone
        const sf::Vector2f getSpawnPoint(const unsigned& team) const;

        //load the given level, indicate success or failure
        bool load(std::string levelName);

        //clear everything currently saved
        void clearWorld();

        //update the position of the given that the given player may have
        void updateFlagPosition(PlayerBase& player);

        //draw the objects of the given floor
        void drawBackground(sf::RenderWindow& window, const unsigned& floor);
        void drawForeground(sf::RenderWindow& window, const unsigned& floor);
};

#endif // GAMEWORLD_H_INCLUDED
