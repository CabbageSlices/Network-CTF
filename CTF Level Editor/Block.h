#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

#include "StaticObject.h"

class Block : public StaticObject {

    protected:

        virtual void setupCollisionBox(const sf::Vector2f& size);

    public:

        Block(const sf::Vector2f& blockSize);
        void setPosition(const sf::Vector2f& position);

        virtual ~Block();
};

#endif // BLOCK_H_INCLUDED
