#include "Lerp.h"
#include <iostream>

const float interpolate(const float& currentValue, const float& destinationValue, const float& deltaFraction) {

    //calculate what fraction of the total distance should be moved
    float totalDistance = destinationValue - currentValue;

    //if the delta fraction is greater than one it will begin extrapolating and we don't want that so limit it to one
    float distanceToCover = totalDistance * (deltaFraction < 1 ? deltaFraction : 1);

    return distanceToCover;
}
