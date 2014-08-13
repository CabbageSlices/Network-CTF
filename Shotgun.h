#ifndef SHOTGUN_H_INCLUDED
#define SHOTGUN_H_INCLUDED

#include "Gun.h"

class Shotgun : public Gun {

    private:

        //number of bullets shotgun should fire
        //no matter the damage done by the shotgun bullets if you're hit by all of them
        //you should get killed
        const int bulletsFired;

    public:

        Shotgun();
        virtual ~Shotgun(){}

        virtual void updateGunfire(const sf::Time& delta);
};

#endif // SHOTGUN_H_INCLUDED
