#include "Minimap.h"

Minimap::Minimap(const sf::Vector2u& windowSize):
    view(),
    borderTexture(),
    borderSprite(),
    viewingAreaOffset(27, 43),
    viewingAreaSize(242, 158),
    levelSize(1024, 768),
    movementOffset(0.01)
    {
        borderTexture.loadFromFile("minimap.png");
        borderSprite.setTexture(borderTexture);

        setupBorderPosition(windowSize);

        setupViewingRegion(windowSize);

        //set level size to the default level size
        setLevelSize(levelSize);

        handleScreenResize(windowSize);
    }

void Minimap::setLevelSize(const sf::Vector2f& size) {

    //keep the aspect ratio of the level on the minimap but make it more zoomed in
    levelSize = size;
    view.setSize(levelSize.x / 2.5, levelSize.y / 2.5);
}

void Minimap::applyMinimap(sf::RenderWindow& window) const {

    window.setView(view);
}

void Minimap::handleScreenResize(const sf::Vector2u& windowSize) {

    setupBorderPosition(windowSize);
    setupViewingRegion(windowSize);
}

void Minimap::drawBorder(sf::RenderWindow& window) {

    window.draw(borderSprite);
}

void Minimap::setupViewingRegion(const sf::Vector2u& windowSize) {

    float viewXPosition = (borderSprite.getPosition().x + viewingAreaOffset.x) / static_cast<float>(windowSize.x);
    float viewYPosition = (borderSprite.getPosition().y + viewingAreaOffset.y) / static_cast<float>(windowSize.y);
    float viewWidth = viewingAreaSize.x / static_cast<float>(windowSize.x);
    float viewHeight = viewingAreaSize.y / static_cast<float>(windowSize.y);

    view.setViewport(sf::FloatRect(viewXPosition, viewYPosition, viewWidth, viewHeight));
}

void Minimap::setupBorderPosition(const sf::Vector2u& windowSize) {

    sf::Vector2f borderPosition(windowSize.x - borderSprite.getGlobalBounds().width,
                                windowSize.y - borderSprite.getGlobalBounds().height);

    borderSprite.setPosition(borderPosition);
}
