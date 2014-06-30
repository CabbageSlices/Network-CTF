#include "GameManager.h"

GameManager::GameManager() :
    timer(),
    accumulator(),
    optimalTimeStep(sf::milliseconds(15))
    {

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

        accumulator -= optimalTimeStep;
    }

    this->handlePostUpdate();
}

void GameManager::drawWorld(sf::RenderWindow& window) {

    window.clear();

    this->drawComponents(window);

    window.display();
}

const float GameManager::calculateDeltaFraction() {

    return accumulator.asSeconds() / optimalTimeStep.asSeconds();
}
