#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

#include "StaticObject.h"

class Block : public StaticObject {

    protected:

        virtual void setupCollisionBox(const sf::Vector2f& size);

        //for special blocks that allow bullets to be shot through ,like the ones placed over water
        bool letBulletsPass;

    public:

        Block(const sf::Vector2f& blockSize, const bool& passBullets = false);
        void setPosition(const sf::Vector2f& position);
        void setPassBullets(const bool& passBullets);

        bool getPassBullets();

        virtual ~Block();
};

#endif // BLOCK_H_INCLUDED
