#include "PredrawnButton.h"

using std::string;

PredrawnButton::PredrawnButton(const std::string& imagePath):
    texture(),
    sprite(),
    isMouseTouching(false)
    {
        texture.loadFromFile(imagePath);
        sprite.setTexture(texture);

        //set the origin to the center of the image that way
        //when you're setting the position the button is automatically centered
        sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);

    }

void PredrawnButton::setPosition(const sf::Vector2f& position) {

    sprite.setPosition(position);
}

bool PredrawnButton::checkMouseTouching(const sf::Vector2f& mousePosition) {

    isMouseTouching = sprite.getGlobalBounds().contains(mousePosition);

    //when mouse is touching the button make it darker for effect
    if(isMouseTouching) {

        sprite.setColor(sf::Color(150, 150, 150, 255));

    } else {

        sprite.setColor(sf::Color(255, 255, 255, 255));
    }

    return isMouseTouching;
}

void PredrawnButton::draw(sf::RenderWindow& window) {

    window.draw(sprite);
}