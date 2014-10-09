#include "HeadsUpDisplay.h"

HeadsUpDisplay::HeadsUpDisplay(const sf::Vector2f& minimapSize, const sf::Vector2u& screenSize) :
    minimap(minimapSize, screenSize),
    statDisplay(screenSize),
    scoreDisplay(screenSize)
    {

    }
