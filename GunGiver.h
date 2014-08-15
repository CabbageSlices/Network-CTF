#ifndef GUNGIVER_H_INCLUDED
#define GUNGIVER_H_INCLUDED

#include "GunTypes.h"
#include "StaticObject.h"

class PlayerBase;

//object that gives a gun to the person who touches it, or reloads their ammo if they already have the gun
class GunGiver : public StaticObject {

    private:

        GunTypes gunToGive;

    public:

        GunGiver(const sf::Vector2f& centerPosition, const GunTypes& gun);
        virtual ~GunGiver(){}

        virtual void handleCollision(PlayerBase& collidingPlayer);
};

#endif // GUNGIVER_H_INCLUDED
