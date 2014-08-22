#ifndef FLOOR_H_INCLUDED
#define FLOOR_H_INCLUDED

#include <vector>
#include <tr1/memory>

#include "LargeImage.h"

class Block;
class ForegroundObject;
class Portal;
class GunGiver;

//objects that exists within a floor
struct Floor {

    std::vector<std::tr1::shared_ptr<Block> > blocks;
    std::vector<std::tr1::shared_ptr<ForegroundObject> > foregroundObjects;
    std::vector<std::tr1::shared_ptr<Portal> > portals;
    std::vector<std::tr1::shared_ptr<GunGiver> > gunGivers;

    //background image of the floor
    LargeImage backgroundImage;
};

#endif // FLOOR_H_INCLUDED
