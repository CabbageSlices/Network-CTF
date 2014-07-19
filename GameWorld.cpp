#include "GameWorld.h"
#include "Block.h"
#include "LevelManager.h"

using std::vector;
using std::tr1::shared_ptr;
using std::string;

GameWorld::GameWorld():
    blocks()
    {

    }

vector<shared_ptr<Block> >& GameWorld::getBlocks() {

    return blocks;
}

bool GameWorld::load(string levelName) {

    return loadLevel(levelName, blocks);
}

void GameWorld::clearWorld() {

    //clear all entities
    blocks.clear();
}

void GameWorld::drawBackground(sf::RenderWindow& window) {

    for(auto block : blocks) {

        block->draw(window);
    }
}
