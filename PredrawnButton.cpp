#include "PredrawnButton.h"

using std::string;

PredrawnButton::PredrawnButton(const std::string& imagePath):
    texture(),
    sprite(),
    cover(sf::Vector2f(1, 1)),
    isMouseTouching(false)
    {
        texture.loadFromFile(imagePath);
        sprite.setTexture(texture);

        //set the origin to the center of the image that way
        //when you're setting the position the button is automatically centered
        sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);

        cover.setFillColor(sf::Color(0, 0, 0, 100));

        resetCover();
    }

void PredrawnButton::setPosition(const sf::Vector2f& position) {

    sprite.setPosition(position);
    cover.setPosition(position);
}

bool PredrawnButton::checkMouseTouching(const sf::Vector2f& mousePosition) {

    isMouseTouching = sprite.getGlobalBounds().contains(mousePosition);

    return isMouseTouching;
}

void PredrawnButton::draw(sf::RenderWindow& window) {

    window.draw(sprite);

    if(isMouseTouching) {

        window.draw(cover);
    }
}

void PredrawnButton::resizeCover() {

    cover.setSize(sf::Vector2f(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height));
    cover.setOrigin(cover.getLocalBounds().width / 2, cover.getLocalBounds().height / 2);
}

void PredrawnButton::repositionCover() {

    cover.setPosition(sprite.getPosition());
}
