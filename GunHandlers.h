#ifndef GUNHANDLERS_H_INCLUDED
#define GUNHANDLERS_H_INCLUDED

#include "GunTypes.h"

#include <tr1/memory>

class Gun;

//functions that work with guns
bool areSameGuns(std::tr1::shared_ptr<Gun> gun1, std::tr1::shared_ptr<Gun> gun2);

//checks if the given gun is the given type
bool isType(const std::tr1::shared_ptr<Gun> gun1, const GunTypes& newGunType);

std::tr1::shared_ptr<Gun> createGun(const GunTypes& gunType);

#endif // GUNHANDLERS_H_INCLUDED
