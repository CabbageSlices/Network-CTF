#include "OnOffButton.h"

OnOffButton::OnOffButton(bool on):
    PredrawnButton("images/soundButtons.png"),
    onClip(0, 0, 63, 24),
    offClip(65, 0, 63, 24),
    isOn(on)
    {
        sprite.setTextureRect(onClip);

        sprite.setOrigin(onClip.width / 2, onClip.height / 2);

        sprite.setTextureRect(offClip);

        if(isOn) {

            sprite.setTextureRect(onClip);
        }
    }
