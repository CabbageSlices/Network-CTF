#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

class Block {

    private:

        sf::RectangleShape collisionBox;

    public:

        Block(const sf::Vector2f& centerPosition);

        const sf::FloatRect getCollisionBox() const;
        const sf::Vector2f getPosition() const;

        void draw(sf::RenderWindow& window);
};

#endif // BLOCK_H_INCLUDED
