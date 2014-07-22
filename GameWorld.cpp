#include "GameWorld.h"
#include "Block.h"
#include "LevelManager.h"
#include "ForegroundObject.h"

using std::vector;
using std::tr1::shared_ptr;
using std::string;

GameWorld::GameWorld():
    blocks(),
    foregroundObjects()
    {
        shared_ptr<ForegroundObject> newForeground(new ForegroundObject(sf::Vector2f(300, 300)));
        foregroundObjects.push_back(newForeground);

        newForeground.reset(new ForegroundObject(sf::Vector2f(350, 300)));
        foregroundObjects.push_back(newForeground);
    }

vector<shared_ptr<Block> >& GameWorld::getBlocks() {

    return blocks;
}

vector<shared_ptr<ForegroundObject> >& GameWorld::getForeground() {

    return foregroundObjects;
}

bool GameWorld::load(string levelName) {

    return loadLevel(levelName, blocks);
}

void GameWorld::clearWorld() {

    //clear all entities
    blocks.clear();
    foregroundObjects.clear();
}

void GameWorld::drawBackground(sf::RenderWindow& window) {

    for(auto block : blocks) {

        block->draw(window);
    }
}

void GameWorld::drawForeground(sf::RenderWindow& window) {

    for(auto object : foregroundObjects) {

        object->draw(window);
    }
}
