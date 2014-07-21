#include "StaticObject.h"
#include "math.h"

StaticObject::StaticObject(const sf::Vector2f& centerPosition):
    collisionBox(sf::Vector2f(20, 20))
    {
        collisionBox.setOrigin(calculateCenter(collisionBox.getGlobalBounds() ));
        collisionBox.setPosition(centerPosition);

        //setup the size and other information about the static object
        setupCollisionBox();
    }

StaticObject::~StaticObject() {

}

const sf::FloatRect StaticObject::getCollisionBox() const {

    return collisionBox.getGlobalBounds();
}

const sf::Vector2f StaticObject::getPosition() const {

    return collisionBox.getPosition();
}

void StaticObject::draw(sf::RenderWindow& window) {

    window.draw(collisionBox);
}

void StaticObject::setupCollisionBox() {

    collisionBox.setSize(sf::Vector2f(50, 50));
}
