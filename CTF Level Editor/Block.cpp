#include "Block.h"
#include "math.h"

Block::Block(const sf::Vector2f& blockSize):
    StaticObject(sf::Vector2f(0, 0))
    {
        setupCollisionBox(blockSize);
    }

Block::~Block() {

}

void Block::setPosition(const sf::Vector2f& position) {

    collisionBox.setPosition(position);
}

void Block::setupCollisionBox(const sf::Vector2f& size) {

    collisionBox.setSize(size);
    collisionBox.setFillColor(sf::Color(50, 50, 50, 150));
    collisionBox.setOutlineThickness(-3.0);
    collisionBox.setOutlineColor(sf::Color::Red);
}
