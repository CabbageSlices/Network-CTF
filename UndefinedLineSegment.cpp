#include "UndefinedLineSegment.h"

#include <cmath>

using std::min;
using std::max;

UndefinedLineSegment::UndefinedLineSegment(const sf::Vector2f& start, const sf::Vector2f& end, const int& lineIntercept, const std::string& textureFile,
                    const sf::Vector2f& textureCoordinant1, const sf::Vector2f& textureCoordinant2) :
    LineSegment(start, end, UNDEFINED_SLOPE, lineIntercept, textureFile, textureCoordinant1, textureCoordinant2)
    {

    }

UndefinedLineSegment::~UndefinedLineSegment() {


}

bool UndefinedLineSegment::pointExists(const sf::Vector2f& point) const {

    //just check if the y position of the point exists within the range of the line segment because
    //this function is called after the x position of the intersection has been used so its guaranteed to be correct
    bool withinRange = (point.y >= min(getStartPoint().y, getEndPoint().y) && point.y <= max(getStartPoint().y, getEndPoint().y));
    return withinRange;
}

bool UndefinedLineSegment::intersectsParallelSlope(const LineSegment& line, sf::Vector2f& intersectionPoint) const {

    //if they don't share the same intercept then they're not touching
    if(getIntercept() != line.getIntercept()) {

        return false;
    }

    //check if the two lines exists within each other's ranges because they're line segments so they may not occupy the same space
    //make their ranges into rectanles and if they collide it means they exist in the same space and the lines collide
    sf::FloatRect range;
    range.left = 0;
    range.width = 2;
    range.top = min(getStartPoint().y, getEndPoint().y);
    range.height = max(getStartPoint().y, getEndPoint().y) - range.top;

    sf::FloatRect collidingRange;
    collidingRange.left = 0;
    collidingRange.width = 2;
    collidingRange.top = min(line.getStartPoint().y, line.getEndPoint().y);
    collidingRange.height = max(line.getStartPoint().y, line.getEndPoint().y) - collidingRange.top;

    return range.intersects(collidingRange);
}

bool UndefinedLineSegment::intersectsUndefinedSlope(const LineSegment& line, sf::Vector2f& intersectionPoint) const {

    ///if this function is called it means the other line is parallel to this one
    ///because they are both undefined slopes, and parallel lines are always assumed to have no point of intersection
    return false;

    //unefined slope with undefined slope is the same as a parallel slope collisiong
    return intersectsParallelSlope(line, intersectionPoint);
}

bool UndefinedLineSegment::intersectsNormalLine(const LineSegment& line, sf::Vector2f& intersectionPoint) const {

    //since normal lines already handle collision with undefined lines just let the otherl ine handle the intersection detection
    return line.intersects(*this, intersectionPoint);
}
