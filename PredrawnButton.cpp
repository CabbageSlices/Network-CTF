#include "PredrawnButton.h"

using std::string;
using std::tr1::shared_ptr;
using std::vector;

sf::SoundBuffer PredrawnButton::hover;
sf::SoundBuffer PredrawnButton::click;
sf::Sound PredrawnButton::hoverSound;
sf::Sound PredrawnButton::clickSound;

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

        //if the hover or click sound effects have a duration of 0 it means theres no file loaded so load them
        if(hover.getDuration() == sf::seconds(0)) {

            hover.loadFromFile("sounds/buttonhover.wav");
            click.loadFromFile("sounds/buttonPress.wav");

            clickSound.setBuffer(click);
            hoverSound.setBuffer(hover);
        }
    }

void PredrawnButton::setPosition(const sf::Vector2f& position) {

    sprite.setPosition(position);
}

bool PredrawnButton::checkMouseTouching(const sf::Vector2f& mousePosition, bool playSoundEffect) {

    //check if the mouse was being touched before this current check
    //if it was then trigger the hover sound because the mouse started hovering over the button
    bool wasMouseTouching = isMouseTouching;

    isMouseTouching = sprite.getGlobalBounds().contains(mousePosition);

    //when mouse is touching the button make it darker for effect
    if(isMouseTouching) {

        sprite.setColor(sf::Color(150, 150, 150, 255));

        if(!wasMouseTouching && playSoundEffect && GLO_PLAY_SOUNDS) {

            //mouse started being touched this frame so create a sound
            hoverSound.play();
        }

    } else {

        sprite.setColor(sf::Color(255, 255, 255, 255));
    }

    return isMouseTouching;
}

void PredrawnButton::draw(sf::RenderWindow& window) {

    window.draw(sprite);
}
