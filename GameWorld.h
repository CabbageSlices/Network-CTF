#ifndef GAMEWORLD_H_INCLUDED
#define GAMEWORLD_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

#include <vector>
#include <tr1/memory>
#include <string>

class Block;
class ForegroundObject;
class FlagManager;
class PlayerBase;
class Portal;

//class to manage the game world by keeping track of all of the objects and events
class GameWorld {

    private:

        std::vector<std::tr1::shared_ptr<Block> > blocks;
        std::vector<std::tr1::shared_ptr<ForegroundObject> > foregroundObjects;
        std::vector<std::tr1::shared_ptr<Portal> > portals;
        std::tr1::shared_ptr<FlagManager> flagManager;

    public:

        GameWorld();

        std::vector<std::tr1::shared_ptr<Block> >& getBlocks();
        std::vector<std::tr1::shared_ptr<ForegroundObject> >& getForeground();
        std::vector<std::tr1::shared_ptr<Portal> >& getPortals();
        std::tr1::shared_ptr<FlagManager> getFlagManager();

        //load the given level, indicate success or failure
        bool load(std::string levelName);

        //clear everything currently saved
        void clearWorld();

        //update the position of the given that the given player may have
        void updateFlagPosition(PlayerBase& player);

        void drawBackground(sf::RenderWindow& window);
        void drawForeground(sf::RenderWindow& window);
};

#endif // GAMEWORLD_H_INCLUDED
