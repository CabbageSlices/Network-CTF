#include "Block.h"
#include "math.h"

Block::Block(const sf::Vector2f& centerPosition):
    collisionBox(sf::Vector2f(60, 60))
    {
        collisionBox.setOrigin(calculateCenter(collisionBox.getGlobalBounds() ));
        collisionBox.setPosition(centerPosition);
        collisionBox.setFillColor(sf::Color::Blue);
    }

const sf::FloatRect Block::getCollisionBox() const {

    return collisionBox.getGlobalBounds();
}

const sf::Vector2f Block::getPosition() const {

    return collisionBox.getPosition();
}

void Block::draw(sf::RenderWindow& window) {

    window.draw(collisionBox);
}
