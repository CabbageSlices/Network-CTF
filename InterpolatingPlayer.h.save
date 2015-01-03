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


    private:

        //text to draw the players name so you know who this player is
        sf::Text nameText;
        sf::Font font;

        bool shouldDraw;

    protected:

        virtual void drawGun(sf::RenderWindow& window, const unsigned& drawingFloor);

        virtual DrawingState getDrawingState();

    public:

        InterpolatingPlayer();
        virtual ~InterpolatingPlayer(){}

        //sets a rotation for hte player to interpolate towards
        void setInterpolationRotation(const float& newRotation);

        virtual void interpolate(const float& deltaFraction) {

            PlayerBase::interpolate(deltaFraction);

            nameText.setString(playerName);
            nameText.setPosition(currentHitBox.getPosition().x - nameText.getGlobalBounds().width / 2, currentHitBox.getGlobalBounds().top - nameText.getGlobalBounds().height - 18);
        }

        virtual void draw(sf::RenderWindow& window, const unsigned& drawingFloor = 0) {

            PlayerBase::draw(window, drawingFloor);

            if(drawingFloor == getFloor() && getDrawingState() != DYING) {

                window.draw(nameText);
            }
        }

        void enableDrawing() {

            shouldDraw = true;
        }

        void disableDrawing() {

            shouldDraw = false;
        }

        bool isDrawingEnabled() {

            return shouldDraw;
        }

        //fire a gun and place the bullet at the given location
        void fireGun(const sf::Vector2f& bulletBegin, const sf::Vector2f& bulletEnd, const unsigned& floor);
};

#endif // INTERPOLATINGPLAYER_H_INCLUDED
