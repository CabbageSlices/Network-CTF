#include "GunHandlers.h"
#include "Gun.h"
#include "Sniper.h"
#include "Shotgun.h"
#include "Pistol.h"


using std::tr1::shared_ptr;

bool areSameGuns(shared_ptr<Gun> gun1, shared_ptr<Gun> gun2) {

    return gun1->getGunType() == gun2->getGunType();
}

bool isType(std::tr1::shared_ptr<Gun> gun1, const GunTypes& newGunType) {

    return gun1->getGunType() == newGunType;
}


shared_ptr<Gun> createGun(const GunTypes& gunType) {

    if(gunType == SNIPER) {

        shared_ptr<Gun> (new Sniper());

    } else if(gunType == PISTOL) {

        shared_ptr<Gun> (new Pistol());

    } else if(gunType == SHOTGUN) {

        shared_ptr<Gun> (new Shotgun());
    }

    //by efault create a pistol
    return shared_ptr<Gun> (new Pistol());
}
