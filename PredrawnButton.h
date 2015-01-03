#ifndef PREDRAWNBUTTON_H_INCLUDED
#define PREDRAWNBUTTON_H_INCLUDED

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Audio.hpp"

#include "soundSettings.h"

#include <string>
#include <vector>
#include <tr1/memory>

///same as button except the button text and texture are predrawn onto the image file
///instead of drawing a cover over the button, it just makes it darker when mouse touches the button
///because predrawn buttons might not be squares and placing a cover might make a square border around the button
class PredrawnButton {

    protected:

        sf::Texture texture;
        sf::Sprite sprite;

        bool isMouseTouching;

        //button hover sound when user goes over button and the click sound
        //make the sound buffers static so you don't have to load them multiple times
        static sf::SoundBuffer hover;
        static sf::SoundBuffer click;

        //cached sounds which are played when required
        static sf::Sound hoverSound;
        static sf::Sound clickSound;

    public:

        PredrawnButton(const std::string& imagePath);

        virtual ~PredrawnButton() {

        }

        void setPosition(const sf::Vector2f& position);

        //if mouse is touching button it also sets isMouseTouching to true that way it can darken the image for effect
        //also plays sound effect if it is told to play
        //if a button is not on the currently drawn screen then you would need to disable the sound effect
        bool checkMouseTouching(const sf::Vector2f& mousePosition, bool playSound = true);

        virtual void onClick() {

        }

        virtual bool checkIsOn() {

            return true;
        }

        static void playClickSound() {

            if(GLO_PLAY_SOUNDS) {

                clickSound.play();
            }
        }

        void draw(sf::RenderWindow& window);
};

#endif // PREDRAWNBUTTON_H_INCLUDED
