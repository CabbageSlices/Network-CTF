#ifndef PISTOL_H_INCLUDED
#define PISTOL_H_INCLUDED

#include "Gun.h"

class Pistol : public Gun {

    private:

        static sf::SoundBuffer fireBuffer;
        static sf::SoundBuffer reloadBuffer;

        void setupClips();

    protected:

        virtual void handleFireSound() {

            if(GLO_PLAY_SOUNDS) {

                fireSound.play();
            }
        }

    public:

        Pistol();
        virtual ~Pistol(){}
};

#endif // PISTOL_H_INCLUDED
