#include "GameManager.h"
#include "LevelManager.h"

using std::vector;
using std::tr1::shared_ptr;

GameManager::GameManager() :
    timer(),
    accumulator(),
    optimalTimeStep(sf::milliseconds(15)),
    world(),
    minimap(sf::Vector2f(0.25, 0.25), sf::Vector2u(1024, 768)),
    currentWindow()
    {
        world.load("level");
        minimap.setLevelSize(sf::Vector2f(5000, 2000));
    }

void GameManager::runGame(sf::RenderWindow& window) {

    //setup is virtual so this will be a polymorphic call depending on what class calls it
    //first setup for the base game manager, then setup for the derived classes
    GameManager::setup(window);
    this->setup(window);

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

        GameManager::handleWindowEvents(event, window);
        this->handleWindowEvents(event, window);

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

        //after updating objects handle collision between objects
        this->handleCollisions();

        accumulator -= optimalTimeStep;
    }

    this->handlePostUpdate(window);
}

void GameManager::drawWorld(sf::RenderWindow& window) {

    window.clear();

    world.drawBackground(window, getFloor());

    this->drawComponents(window);

    world.drawForeground(window, getFloor());

    GameManager::drawMinimap(window);

    window.display();
}

void GameManager::setupCurrentWindow(sf::RenderWindow& window) {

    currentWindow.setSize(window.getSize().x, window.getSize().y);
    currentWindow.setCenter(window.getSize().x / 2, window.getSize().y / 2);
}

void GameManager::setup(sf::RenderWindow& window) {

    minimap.updateBorder(window.getSize());

    setupCurrentWindow(window);
}

void GameManager::handleWindowEvents(sf::Event& event, sf::RenderWindow& window) {

    if(event.type == sf::Event::Resized) {

        minimap.updateBorder(sf::Vector2u(event.size.width, event.size.height));

        setupCurrentWindow(window);
    }
}

void GameManager::drawMinimap(sf::RenderWindow& window) {

    //get the previous view before minimap is drawn that way it can be reapplied later
    sf::View previousView = window.getView();

    //the minimap border is drawn on the defualt window becausei ts a UI
    window.setView(currentWindow);
    minimap.drawBorder(window);

    //set the minimaps view
    minimap.applyMinimap(window);

    world.drawBackground(window, getFloor());

    //draw the derived class's minimap components
    this->drawMinimap(window);

    world.drawForeground(window, getFloor());

    //re-apply old view
    window.setView(previousView);
}

vector<shared_ptr<Block> >& GameManager::getBlocks(const unsigned& floor) {

    return world.getBlocks(floor);
}

vector<shared_ptr<ForegroundObject> >& GameManager::getForeground(const unsigned& floor) {

    return world.getForeground(floor);
}

vector<shared_ptr<Portal> >& GameManager::getPortals(const unsigned& floor) {

    return world.getPortals(floor);
}

shared_ptr<FlagManager> GameManager::getFlagManager() {

    return world.getFlagManager();
}

GameWorld& GameManager::getGameWorld() {

    return world;
}

const float GameManager::calculateDeltaFraction() {

    return accumulator.asSeconds() / optimalTimeStep.asSeconds();
}
