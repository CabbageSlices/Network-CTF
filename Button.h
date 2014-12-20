#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

#include <string>
#include <functional>

///button objects show text onto a given texture
///when user hovers over them it becomes darker
///the texture used by the button must be supplied as a file
///and the words displayed on the button determines the size of the button
///the button texture should be a 100 by 50 pixel image file which will be resized according to the size of the text
class Button {

    private:

        //image that the button will use
        sf::Texture texture;
        sf::Sprite sprite;
        sf::Font font;

        //borders are the thickness of the border on the button at one edge
        sf::Vector2f borders;

        //message displayed to user
        sf::Text text;

        //cover to draw over the button when user hovers mouse over it
        sf::RectangleShape cover;

        //if mouse is hovering the button then it should get darker so keep
        bool isMouseTouching;

        void setup(const std::string& imagePath, const std::string& fontPath, const std::string& message);

        //resizes the sprite so that the text can fit inside it
        //also resizes cover
        void resizeSprite();

        //resets the position of the sprite so that the text is in the center of the sprite
        //also respositions the cover
        void repositionSprite();

        //fuction that does the actual scaling work
        //the given function pointer is used to handle the text scaling while this function does the work to scale the cover and sprite
        void doScale(const sf::Vector2f& scale, std::function<void(const sf::Vector2f& scale)> scaleFunction);

    public:

        Button(const std::string& imagePath, const std::string& fontPath, const std::string& message, const sf::Vector2f& imageBorders = sf::Vector2f(10, 20));

        void setPosition(const sf::Vector2f& position);
        void setText(const std::string& message);

        void setScale(sf::Vector2f scale);
        void scale(sf::Vector2f textScale);

        //if mouse is touching button it also sets isMouseTouching to true
        bool checkMouseTouching(sf::Vector2f& mousePosition);

        void draw(sf::RenderWindow& window);
};

#endif // BUTTON_H_INCLUDED
