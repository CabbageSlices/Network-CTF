#include "GameWorld.h"
#include "Block.h"
#include "LevelManager.h"
#include "ForegroundObject.h"
#include "FlagManager.h"

using std::vector;
using std::tr1::shared_ptr;
using std::string;

GameWorld::GameWorld():
    blocks(),
    foregroundObjects(),
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

bool GameWorld::load(string levelName) {

    clearWorld();

    flagManager.reset(new FlagManager(sf::Vector2f(0, 0), sf::Vector2f(500, 800)));
    return loadLevel(levelName, blocks, foregroundObjects);
}

void GameWorld::clearWorld() {

    //clear all entities
    blocks.clear();
    foregroundObjects.clear();
    flagManager.reset();
}

void GameWorld::drawBackground(sf::RenderWindow& window) {

    for(auto& block : blocks) {

        block->draw(window);
    }

    flagManager->draw(window);
}

void GameWorld::drawForeground(sf::RenderWindow& window) {

    for(auto& object : foregroundObjects) {

        object->draw(window);
    }
}
