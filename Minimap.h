#ifndef MINIMAP_H_INCLUDED
#define MINIMAP_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

class Minimap {

    private:

        sf::View view;
        sf::RectangleShape border;

        //size of the level that way everything is drawn onto the minimap properly
        sf::Vector2f levelSize;

        //% of the total screen size to move the minimap by from the bototm right of the screen
        const float movementOffset;

    public:

        Minimap(const sf::Vector2f& minimapSize, const sf::Vector2u& windowSize);

        //set the minimap to the size of the level and center the map on the level
        void setLevelSize(const sf::Vector2f& level);

        //apply view to given window
        void applyMinimap(sf::RenderWindow& window) const;

        //if window is resized the minimap border needs to be resized and repositioned
        //the view doesn't need to change because the size is dependent on the level size and the viewport is onlly used by the window
        void updateBorder(const sf::Vector2u& windowSize);

        void drawBorder(sf::RenderWindow& window);
};

#endif // MINIMAP_H_INCLUDED
