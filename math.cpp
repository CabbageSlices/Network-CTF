#include "math.h"
#include <cmath>
#include <cstdlib>

using std::abs;

const float PI = 3.14159265;

float calculateReferenceAngle(const float& angleStandardPosition) {

    //reference angle dependent on quadrant
    if(angleStandardPosition <= 90) {

        return angleStandardPosition;
    }

    if(90 > angleStandardPosition && angleStandardPosition <= 180) {

        return 180 - angleStandardPosition;
    }

    if(180 > angleStandardPosition && angleStandardPosition <= 270) {

        return angleStandardPosition - 180;
    }

    //last case is quandrant 4
    return 360 - angleStandardPosition;
}

float calculateAngle(const sf::Vector2f& beginPoint, const sf::Vector2f& endPoint) {

    //figure out reference angle by using the triangle that forms between the begin and end points
    //because we use tangent you have to make sure the denominator (x value) is not 0
    float vertical = endPoint.y - beginPoint.y;

    //in game world y is positive at the bottom and negative at the top, but the math needs to have y positive at the top and negative at the bottom so inverse the sign
    vertical *= -1;

    float horizontal = endPoint.x - beginPoint.x;

    if(horizontal == 0) {

        //check if the angle is 90 or 270 because when horizontal is 0 it's either pointing straight up or straight down
        //end point is above beginning so hes pointing straight up
        if(endPoint.y <= beginPoint.y) {

            return 90.f;
        }

        //pointing straight down
        return 270.f;
    }

    //angle is calculated in radians to covnert to degree before returning
    float angle = atan2(vertical, horizontal);

    return radiansToDegrees(angle);
}

float degreesToRadians(const float& angleInDegrees) {

    return angleInDegrees * PI / 180.f;
}

float radiansToDegrees(const float& angleInRadians) {

    return angleInRadians * 180.f / PI;
}

float distanceToPoint(const sf::Vector2f& beginPoint, const sf::Vector2f& endPoint) {

    //distance is a^2 + b^2 because this is te same as a triangle
    float horizontal = endPoint.x - beginPoint.x;
    float vertical = endPoint.y - beginPoint.y;

    return horizontal * horizontal + vertical * vertical;
}

sf::Vector2f calculateCenter(const sf::FloatRect& rect) {

    sf::Vector2f center;
    center.x = rect.left + rect.width / 2;
    center.y = rect.top + rect.height / 2;

    return center;
}

int getRand(int max, int min) {

    //if max == 0 you will divide by 0 and it will crash the program so if max is 0 then return 0
    if(max == 0) {

        return max;
    }

    ///see header as to why you are subtracting
    //make sure subtracted is a positive number otherwise the range [max, min] is changed
    int subtracted = min - 1 > 0 ? min - 1 : 0;

    max -= subtracted;
    min -= subtracted;

    return (rand() % max + min) + subtracted;
}

bool compareFloats(const float& float1, const float& float2, const float differenceThreshold) {

    return abs(float1 - float2) < differenceThreshold;
}
