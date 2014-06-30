#ifndef LERP_H_INCLUDED
#define LERP_H_INCLUDED

//interpolate from the given value to the destination value and return how much the given value should increase by in order to reach the destination value
const float interpolate(const float& currentValue, const float& destinationValue, const float& deltaFraction);

#endif // LERP_H_INCLUDED
