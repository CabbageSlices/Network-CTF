#ifndef BUTTONPLACER_H_INCLUDED
#define BUTTONPLACER_H_INCLUDED

#include <vector>
#include <string>

#include "SFML/Graphics.hpp"

//color of the pixel which indicates a button should be placed there
//THE ONLY PART OF THE COLOUR THAT IS COMPARED IS THE TRANSPARECNY
//THAT WAY THE ACTUAL IMAGE FILE DOESN'T HAVE A RANDOM SECTION THATS A COMPLETELY DIFFERENT COLOR FROM REST OF THE IMAGE
//GIMPS TRANSPARECNY FOR BUTTONS SHOULD BE 99%, WHICH IS 253 IN CODE
extern const sf::Color BUTTON_DOT;

//color to replace the button dot color with once a button is placed
extern const sf::Color REPLACEMENT_COLOR;

//gets the center of a button dot and removes it from the image temporarily
sf::Vector2f removeDot(sf::Image& image, const unsigned& startingXPosition, const unsigned& startingYPosition);

///function that takes in a container of buttons
///and centers them onto small dots in a given image file
///the dots must square shaped and must be of the specified color in order for it to be considered a button destination
///the image is scanned starting from the top left and going one row at a time
///every time a button dot is found the center of a button is placed on the center of the dot
///if there are no button dots on the image file then no button will get placed
///imagePath is the file path of the image where the buttons will be placed
///the button type is a template that way the vector can use some form of a pointer that points to a button object

template <class ButtonPointer>
void placeButtons(const std::string& imagePath, std::vector<ButtonPointer>& buttons) {

    sf::Image image;
    image.loadFromFile(imagePath);

    unsigned buttonsIndex = 0;

    for(unsigned pixelYPos = 0; pixelYPos < image.getSize().y && buttonsIndex < buttons.size(); ++pixelYPos) {

        for(unsigned pixelXPos = 0; pixelXPos < image.getSize().x && buttonsIndex < buttons.size(); ++pixelXPos) {

            //the beginning of a button dot was found so now you have to calculate the center of the dot
            //in addition you also have to remove the entire dot that way you won't find it again on the next loop
            if(image.getPixel(pixelXPos, pixelYPos).a == BUTTON_DOT.a) {

                sf::Vector2f buttonCenter = removeDot(image, pixelXPos, pixelYPos);

                //a button pixel was found so take the current button being placed, and center it at this position, then move on to the next button
                buttons[buttonsIndex]->setPosition(buttonCenter);
                buttonsIndex += 1;
            }
        }
    }
}

#endif // BUTTONPLACER_H_INCLUDED
