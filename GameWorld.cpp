#include "GameWorld.h"
#include "Block.h"
#include "LevelManager.h"
#include "ForegroundObject.h"
#include "FlagManager.h"
#include "TeamManager.h"
#include "PlayerBase.h"
#include "Portal.h"

using std::vector;
using std::tr1::shared_ptr;
using std::string;

GameWorld::GameWorld():
    blocks(),
    foregroundObjects(),
    portals(),
    flagManager()
    {

    }

vector<shared_ptr<Block> >& GameWorld::getBlocks() {

    return blocks;
}

vector<shared_ptr<ForegroundObject> >& GameWorld::getForeground() {

    return foregroundObjects;
}

shared_ptr<FlagManager> GameWorld::getFlagManager() {

    return flagManager;
}

vector<shared_ptr<Portal> >& GameWorld::getPortals() {

    return portals;
}

bool GameWorld::load(string levelName) {

    clearWorld();

    flagManager.reset(new FlagManager(sf::Vector2f(0, 0), sf::Vector2f(500, 800)));
    portals.push_back(shared_ptr<Portal>(new Portal(sf::Vector2f(-200, 200), sf::Vector2f(600, 800)))   );

    return loadLevel(levelName, blocks, foregroundObjects);
}

void GameWorld::clearWorld() {

    //clear all entities
    blocks.clear();
    foregroundObjects.clear();
    flagManager.reset();
}

void GameWorld::updateFlagPosition(PlayerBase& player) {

    if(player.isHoldingFlag()) {

        flagManager->setFlagPosition(player.getCurrentPosition(), getOpposingTeam(player.getTeam()));
    }
}

void GameWorld::drawBackground(sf::RenderWindow& window) {

    for(auto& block : blocks) {

        block->draw(window);
    }

    for(auto& portal : portals) {

        portal->draw(window);
    }

    flagManager->draw(window);
}

void GameWorld::drawForeground(sf::RenderWindow& window) {

    for(auto& object : foregroundObjects) {

        object->draw(window);
    }
}
