#ifndef GUNGIVER_H_INCLUDED
#define GUNGIVER_H_INCLUDED

#include "GunTypes.h"
#include "StaticObject.h"

#include <map>

//object that gives a gun to the person who touches it, or reloads their ammo if they already have the gun
class GunGiver : public StaticObject {

    private:

        GunTypes gunToGive;

        //all possible textures for gungivers
        static std::map<GunTypes, sf::Texture> textures;

        //whether the textures are created
        static bool texturesCreated;

    void initializeTextures();

    public:

        GunGiver(const GunTypes& gun);
        virtual ~GunGiver(){}

        GunTypes getGunType() const;

        void setPosition(const sf::Vector2f& position);
};

#endif // GUNGIVER_H_INCLUDED
