#include "ButtonPlacer.h"
#include "SFML/Graphics.hpp"

const sf::Color BUTTON_DOT = sf::Color(174, 29, 113, 253);
const sf::Color REPLACEMENT_COLOR = sf::Color(173, 28, 112, 255);

sf::Vector2f removeDot(sf::Image& image, const unsigned& startingXPosition, const unsigned& startingYPosition) {

    sf::FloatRect dotBoundingBox(startingXPosition, startingYPosition, 0, 0);

    //dot is always a square so starting from the given position
    //first figure out size of the dot that way later you can replace the dot with a different color
    //scan left and scan down to determine how long the dot is on each axis

    //determine width
    for(unsigned xPos = startingXPosition; (xPos < image.getSize().x) && image.getPixel(xPos, startingYPosition).a == BUTTON_DOT.a; ++xPos) {

        //every pixel that is the same color as the dot lets you increase the width by 1
        dotBoundingBox.width += 1;
    }

    //determine height
    for(unsigned yPos = startingYPosition; yPos < image.getSize().y && image.getPixel(startingXPosition, yPos).a == BUTTON_DOT.a; ++yPos) {

        //every pixel that is the same color as the dot lets you increase the heighth by 1
        dotBoundingBox.height += 1;
    }

    //now replace the dot's transparency with the other color's transparency
    for(unsigned yPos = startingYPosition; (yPos - startingYPosition) <= dotBoundingBox.height; ++yPos) {

        for(unsigned xPos = startingXPosition; (xPos - startingXPosition) <= dotBoundingBox.width; ++xPos) {

            sf::Color pixel = image.getPixel(xPos, yPos);
            image.setPixel(xPos, yPos, sf::Color(pixel.r, pixel.g, pixel.b, REPLACEMENT_COLOR.a));
        }
    }

    //now calculate the center and return it
    sf::Vector2f center(dotBoundingBox.left + dotBoundingBox.width / 2, dotBoundingBox.top + dotBoundingBox.height / 2);

    return center;
}
