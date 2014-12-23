#ifndef PISTOL_H_INCLUDED
#define PISTOL_H_INCLUDED

#include "Gun.h"

class Pistol : public Gun {

    private:

        void setupClips();

    public:

        Pistol();
        virtual ~Pistol(){}
};

#endif // PISTOL_H_INCLUDED
