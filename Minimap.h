#ifndef MINIMAP_H_INCLUDED
#define MINIMAP_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

/*
size of the minimap is dependent on the size of the minimap texture
however its not based on the size of the image, just the size of the viewing region alloted in the image file
the size is represented by viewAreaSize
*/
class Minimap {

    private:

        sf::View view;

        sf::Texture borderTexture;
        sf::Sprite borderSprite;

        //distance from the origin of the border to the actual place where the map can be drawn
        //expressed in pixels, not in percent of the screen
        //this is the offset to the viewing area depicted on the imagefile of the border
        sf::Vector2f viewingAreaOffset;

        //size of the viewing area, which is different from the border size, expressed in pixels
        sf::Vector2f viewingAreaSize;

        //size of the level that way everything is drawn onto the minimap properly
        sf::Vector2f levelSize;

        //% of the total screen size to move the minimap by from the bototm right of the screen
        const float movementOffset;

        //calculates what fraction of the screen the border sprite takes up
        //and uses that to calculate the view for the minimap
        void setupViewingRegion(const sf::Vector2u& windowSize);

        //place the entier minimap border at the bottomright of the screen
        void setupBorderPosition(const sf::Vector2u& windowSize);

    public:

        Minimap(const sf::Vector2u& windowSize);

        //set the minimap to the size of the level and center the map on the level
        void setLevelSize(const sf::Vector2f& level);

        //apply view to given window
        void applyMinimap(sf::RenderWindow& window) const;

        //if window is resized the minimap border needs to be resized and repositioned
        //the view doesn't need to change because the size is dependent on the level size and the viewport is onlly used by the window
        void handleScreenResize(const sf::Vector2u& windowSize);

        //sets the point that the minimap should zoom in on
        void setTarget(const sf::Vector2f& target) {

            view.setCenter(target);

            //make sure the view doesn't go out of level bounds
            //if its out of bounds move it onto the level
            if(target.x - view.getSize().x / 2  < 0) {

                view.move(view.getSize().x / 2 - target.x, 0);
            }

            if(target.y - view.getSize().y / 2 < 0) {

                view.move(0, view.getSize().y / 2 - target.y);
            }

            if(target.x + view.getSize().x / 2 > levelSize.x) {

                view.move(-((target.x + view.getSize().x / 2) - levelSize.x), 0);
            }

            if(target.y + view.getSize().y / 2 > levelSize.y) {

                view.move(0, -((target.y + view.getSize().y / 2) - levelSize.y));
            }
        }

        void drawBorder(sf::RenderWindow& window);
};

#endif // MINIMAP_H_INCLUDED
