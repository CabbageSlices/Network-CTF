#ifndef INTERPOLATINGPLAYER_H_INCLUDED
#define INTERPOLATINGPLAYER_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "PlayerBase.h"

#include <vector>
#include <tr1/memory>

//a class to represent the other players currently connected to the server
//these players cannot update on the client and will onlly be used for drawing
class InterpolatingPlayer : public PlayerBase {

    protected:

        virtual void drawGun(sf::RenderWindow& window);

    public:

        InterpolatingPlayer();
        ~InterpolatingPlayer(){}

        //sets a rotation for hte player to interpolate towards
        void setInterpolationRotation(const float& newRotation);

        //fire a gun and place the bullet at the given location
        void fireGun(const sf::Vector2f& bulletBegin, const sf::Vector2f& bulletEnd);
};

#endif // INTERPOLATINGPLAYER_H_INCLUDED
