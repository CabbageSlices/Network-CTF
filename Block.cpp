#include "Block.h"

Block::Block(const sf::Vector2f& centerPosition):
    StaticObject(centerPosition)
    {
        setupCollisionBox();
    }

Block::~Block() {

}

void Block::setupCollisionBox() {

    collisionBox.setSize(sf::Vector2f(60, 60));
    collisionBox.setFillColor(sf::Color::Blue);
    collisionBox.setOutlineThickness(-2.0);
    collisionBox.setOutlineColor(sf::Color::Red);
}
