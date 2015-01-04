#ifndef GUNGIVER_H_INCLUDED
#define GUNGIVER_H_INCLUDED

#include "GunTypes.h"
#include "StaticObject.h"
#include "GunHandlers.h"

#include <map>

class PlayerBase;

//object that gives a gun to the person who touches it, or reloads their ammo if they already have the gun
class GunGiver : public StaticObject {

    private:

        GunTypes gunToGive;

        //whether the gungiver should tell the user to right click in order to pick up gun
        bool shouldDrawMessage;

        //all possible textures for gungivers
        static std::map<GunTypes, sf::Texture> textures;

        //whether the textures are created
        static bool texturesCreated;

        //texture for messages
        static sf::Texture messageTextures;

        //each type of gun giver has its own message to display
        static std::map<GunTypes, sf::Sprite> messageSprites;

        void initialize();

    public:

        GunGiver(const GunTypes& gun);
        virtual ~GunGiver(){}

        GunTypes getGunType() const;

        void setPosition(const sf::Vector2f& position);
        virtual void handleCollision(PlayerBase& collidingPlayer);

        void handleRefillAmmo(PlayerBase& collidingPlayer);

        void drawMessage(sf::RenderWindow& window) {

            if(shouldDrawMessage) {

                window.draw(messageSprites[gunToGive]);
            }
        }

        void setDrawMessage(bool shouldDraw) {

            shouldDrawMessage = shouldDraw;
        }
};

#endif // GUNGIVER_H_INCLUDED
