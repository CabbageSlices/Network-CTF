#include "GameManager.h"
#include "Block.h"
#include "LevelManager.h"

using std::vector;
using std::tr1::shared_ptr;

GameManager::GameManager() :
    timer(),
    accumulator(),
    optimalTimeStep(sf::milliseconds(15)),
    blocks()
    {
        loadLevel("level", blocks);
    }

void GameManager::runGame(sf::RenderWindow& window) {

    //setup is virtual so this will be a polymorphic call depending on what class calls it
    this->setup();

    sf::Event event;

    while(window.isOpen()) {

        handleInputs(event, window);

        updateWorld(window);

        drawWorld(window);
    }
}

void GameManager::handleInputs(sf::Event& event, sf::RenderWindow& window) {

    while(window.pollEvent(event)) {

        handleCommonInputs(event, window);

        this->handleComponentInputs(event, window);
    }

    this->handleStateInputs();
}

void GameManager::handleCommonInputs(sf::Event& event, sf::RenderWindow& window) {

    if(event.type == sf::Event::Closed) {

        window.close();
    }
}

void GameManager::updateWorld(sf::RenderWindow& window) {

    this->updateComponents(window);

    //calculate the time passed since the last frame
    accumulator += timer.restart();

    while(accumulator > optimalTimeStep) {

        float delta = optimalTimeStep.asSeconds();

        this->updateTimeComponents(delta, window);

        //after updating objects handle collision between objects
        this->handleCollisions();

        accumulator -= optimalTimeStep;
    }

    this->handlePostUpdate();
}

void GameManager::drawWorld(sf::RenderWindow& window) {

    window.clear();

    this->drawComponents(window);
    drawBlocks(window);

    window.display();
}

void GameManager::drawBlocks(sf::RenderWindow& window) {

    for(auto block : blocks) {

        block->draw(window);
    }
}

vector<shared_ptr<Block> >& GameManager::getBlocks() {

    return blocks;
}

const float GameManager::calculateDeltaFraction() {

    return accumulator.asSeconds() / optimalTimeStep.asSeconds();
}
