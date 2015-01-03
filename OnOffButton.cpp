#include "OnOffButton.h"

OnOffButton::OnOffButton():
    PredrawnButton("soundButtons.png"),
    onClip(0, 0, 63, 24),
    offClip(65, 0, 63, 24),
    isOn(true)
    {
        sprite.setTextureRect(onClip);

        sprite.setOrigin(onClip.width / 2, onClip.height / 2);
    }
