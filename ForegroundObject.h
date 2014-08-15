#ifndef FOREGROUNDOBJECT_H_INCLUDED
#define FOREGROUNDOBJECT_H_INCLUDED

#include "StaticObject.h"
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

class UserPlayer;

//foreground objects are things that are placed on the foreground dthat players can hide behind
class ForegroundObject : public StaticObject {

    private:

        //if player collides with this object then he is hiding behind this object
        bool hidingPlayer;

        virtual void setupCollisionBox();

        void makeSeethrough();
        void makeSolid();

    public:

        ForegroundObject(const sf::Vector2f& centerPosition);

        void setHidingPlayer(const bool& playerHidden);
        virtual void handleCollision(UserPlayer& collidingPlayer);
};

#endif // FOREGROUNDOBJECT_H_INCLUDED
