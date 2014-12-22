#ifndef PREDRAWNBUTTON_H_INCLUDED
#define PREDRAWNBUTTON_H_INCLUDED

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

#include <string>

///same as button except the button text and texture are predrawn onto the image file
///instead of drawing a cover over the button, it just makes it darker when mouse touches the button
///because predrawn buttons might not be squares and placing a cover might make a square border around the button
class PredrawnButton {

    private:

        sf::Texture texture;
        sf::Sprite sprite;

        bool isMouseTouching;

    public:

        PredrawnButton(const std::string& imagePath);

        void setPosition(const sf::Vector2f& position);

        //if mouse is touching button it also sets isMouseTouching to true that way it can darken the image for effect
        bool checkMouseTouching(const sf::Vector2f& mousePosition);

        void draw(sf::RenderWindow& window);
};

#endif // PREDRAWNBUTTON_H_INCLUDED
