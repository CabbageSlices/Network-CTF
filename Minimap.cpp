#include "Minimap.h"

Minimap::Minimap(const sf::Vector2f& minimapSize, const sf::Vector2u& windowSize):
    view(),
    border(),
    levelSize(1024, 768),
    movementOffset(0.01)
    {
        //position the minmap at the bottom right of the screen, subtract a small amount from everything in order to move it slightly to the left so you can see the edges
        view.setViewport(sf::FloatRect(1 - minimapSize.x - movementOffset, 1 - minimapSize.y - movementOffset,
                                        minimapSize.x - movementOffset, minimapSize.y - movementOffset));

        border.setOutlineThickness(3.0);
        border.setOutlineColor(sf::Color::Green);
        border.setFillColor(sf::Color::Black);

        //set level size to the default level size
        setLevelSize(levelSize);

        updateBorder(windowSize);
    }

void Minimap::setLevelSize(const sf::Vector2f& size) {

    levelSize = size;
    view.setSize(levelSize);

    //center the view at the center of the level
    view.setCenter(levelSize.x / 2, levelSize.y / 2);
}

void Minimap::applyMinimap(sf::RenderWindow& window) const {

    window.setView(view);
}

void Minimap::updateBorder(const sf::Vector2u& windowSize) {

    sf::Vector2f borderSize(windowSize.x * view.getViewport().width, windowSize.y * view.getViewport().height);
    border.setSize(borderSize);

    sf::Vector2f borderPosition(windowSize.x * view.getViewport().left, windowSize.y * view.getViewport().top);
    border.setPosition(borderPosition);
}

void Minimap::drawBorder(sf::RenderWindow& window) {

    window.draw(border);
}
