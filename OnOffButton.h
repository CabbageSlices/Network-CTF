#ifndef ONOFFBUTTON_H_INCLUDED
#define ONOFFBUTTON_H_INCLUDED

#include "PredrawnButton.h"

//Button that can turn on or off

class OnOffButton : public PredrawnButton {

    private:

        //clips for on and off on the image texture
        sf::IntRect onClip;
        sf::IntRect offClip;

        bool isOn;

    public:

        OnOffButton(bool on);
        virtual ~OnOffButton() {

        }

        virtual void onClick() {

            isOn = !isOn;

            sprite.setTextureRect(offClip);

            if(isOn) {

                sprite.setTextureRect(onClip);
            }
        }

        virtual bool checkIsOn() {

            return isOn;
        }
};

#endif // ONOFFBUTTON_H_INCLUDED
