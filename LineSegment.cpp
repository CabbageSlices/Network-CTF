#include "LineSegment.h"
#include "UndefinedLineSegment.h"

#include <cmath>
#include <iostream>

using std::cout;
using std::endl;
using std::max;
using std::min;
using std::abs;
using std::tr1::shared_ptr;
using std::string;

const float UNDEFINED_SLOPE = 9999999;
const int MAX_SLOPE_SIZE = 95;

shared_ptr<LineSegment> createLine(const sf::Vector2f& start, const sf::Vector2f& end, const std::string& textureFile,
                    const sf::Vector2f& textureCoordinant1, const sf::Vector2f& texturecoordinant2) {

    float lineSlope = calculateSlope(start, end);

    //cut off the decimal of the slope after 3 decimal places so floating point comparisons actually work
    float roundedSlope = floorf(lineSlope * 1000) / 1000.f;

    float intercept = calculateIntercept(start, roundedSlope);

    //if the slope is special (extreamly large or small) then create a special undefined line
    if(abs(roundedSlope) >= MAX_SLOPE_SIZE) {

        shared_ptr<LineSegment> undefinedLine(new UndefinedLineSegment(start, end, intercept, textureFile, textureCoordinant1, texturecoordinant2));

        return undefinedLine;
    }

    shared_ptr<LineSegment> line(new LineSegment(start, end, roundedSlope, intercept, textureFile, textureCoordinant1, texturecoordinant2));

    return line;
}

std::tr1::shared_ptr<LineSegment> createLine(const sf::Vector2f& start, const sf::Vector2f& end) {

    return createLine(start, end, "", sf::Vector2f(0, 0), sf::Vector2f(0, 0));
}

float calculateSlope(const sf::Vector2f& start, const sf::Vector2f& end) {

    //if the denominator would equal 0 then just return an undefined slope
    if(start.x == end.x) {

        return UNDEFINED_SLOPE;
    }

    float slope = (start.y - end.y) / (start.x - end.x);

    return slope;
}

float calculateIntercept(const sf::Vector2f& start, const float& slope) {

    //only x intercept exists for undefined slopes
    if(slope == UNDEFINED_SLOPE) {

        return start.x;
    }

    //normal line so calculate intercept
    float intercept = start.y - slope * start.x;

    return intercept;
}

LineSegment::LineSegment(const sf::Vector2f& start, const sf::Vector2f& end, const float& lineSlope, const int& lineIntercept, const string& textureFile,
                    const sf::Vector2f& textureCoordinant1, const sf::Vector2f& texturecoordinant2) :
    startPoint(start),
    endPoint(end),
    slope(lineSlope),
    intercept(lineIntercept),
    lineShape(sf::Lines, 2),
    texture()
    {
        lineShape[0].position = startPoint;
        lineShape[1].position = endPoint;
        lineShape[0].texCoords = sf::Vector2f(1, 0);
        lineShape[1].texCoords = sf::Vector2f(100, 0);

        if(textureFile != "") {

            texture.loadFromFile(textureFile);
        } else {

            //no texture so its probably a bullet so make it fade out
            lineShape[1].color = sf::Color(255, 255, 255, 25);
        }

    }

LineSegment::~LineSegment() {

}

const float LineSegment::getSlope() const {

    return slope;
}

const int LineSegment::getIntercept() const {

    return intercept;
}

const sf::Vector2f LineSegment::getStartPoint() const {

    return startPoint;
}

const sf::Vector2f LineSegment::getEndPoint() const {

    return endPoint;
}

void LineSegment::setEndPoint(const sf::Vector2f newEndPoint) {

    endPoint = newEndPoint;
    lineShape[1].position = newEndPoint;
}

bool LineSegment::intersects(const LineSegment& line, sf::Vector2f& intersectionPoint) const {

    //first check if the other line has an undefined slope
    //because those are special cases and their intercpets behave differently
    ///if this->slope equals UNDEFINED_SLOPE then this will result in a parallel line intersection test which will always return false
    if(line.getSlope() == UNDEFINED_SLOPE) {

        //this line shouldn't have an undefined slope because lines with undefined slopes are
        //created as a special line and if this function is being called it means the calling object does not have an undefined slope
        return this->intersectsUndefinedSlope(line, intersectionPoint);
    }

    //parallel lines are either always touching or never touching so determine if they're touching or not
    bool isParallel = (line.getSlope() == slope);

    ///parallel lines always assumed to not intersect because you can't determine point of intersection
    if(isParallel) {

        ///return this->intersectsParallelSlope(line);
        return false;
    }

    //not parallel or undefined line so it must be a normal line, check intersection with normal line
    return this->intersectsNormalLine(line, intersectionPoint);
}

void LineSegment::draw(sf::RenderWindow& window) {

    window.draw(lineShape, &texture);
}

bool LineSegment::pointExists(const sf::Vector2f& point) const {

    //for normal lines simply checking if the x value fits the domain is good enough because this function is usually called after calculating the y value
    //so it's assumed to be valid, the only thing that needs to be checked is if the point is extraneious, meaning if the line cuts off before it reaches the point
    bool withinDomain = (point.x >= min(startPoint.x, endPoint.x) && point.x <= max(startPoint.x, endPoint.x) );

    return withinDomain;
}

bool LineSegment::intersectsParallelSlope(const LineSegment& line, sf::Vector2f& intersectionPoint) const {

    //parallel lines are guaranteed to not intersect if their intercepts are different
    if(line.getIntercept() != intercept) {

        return false;
    }

    //check if the two lines actually exist at the parts where they overlap because these are just line segments so the peice of line may not actually touch
    //to check if they exist within each other's domain, create a rectangle from each lines respective start point to it's respective end point, however only use the x values
    //and make the height of the rect equal to any number other than 0, then if the two rectangle collide it means the lines exist within each other's domain so they intersect
    sf::FloatRect lineDomain;
    lineDomain.left = min(startPoint.x, endPoint.x);
    lineDomain.width = max(startPoint.x, endPoint.x) - lineDomain.left;
    lineDomain.top = 0;
    lineDomain.height = 2;

    sf::FloatRect collidingDomain;
    collidingDomain.left = min(line.getStartPoint().x, line.getEndPoint().x);
    collidingDomain.width = max(line.getStartPoint().x, line.getEndPoint().x) - collidingDomain.left;
    collidingDomain.top = 0;
    collidingDomain.height = 2;

    return lineDomain.intersects(collidingDomain);
}

bool LineSegment::intersectsUndefinedSlope(const LineSegment& line, sf::Vector2f& intersectionPoint) const {

    //other line has undefined slope so just use the x position of the line and plug into the equation of this line in order to calculate where the collision would occur
    //x position of the line is the xIntercept
    float xIntercept = line.getStartPoint().x;

    //calculate y position of intersection
    float yPosition = calculateOutput(xIntercept);

    intersectionPoint.x = xIntercept;
    intersectionPoint.y = yPosition;

    return intersectionExists(line, intersectionPoint);
}

bool LineSegment::intersectsNormalLine(const LineSegment& line, sf::Vector2f& intersectionPoint) const {

    //all normal lines are guaranteed to intersect at some point, you just have to figure out if the lines ever reach the poin of intersection
    //calculate the x position of the point of intersection to determine if the point fits the domain of both line segments
    ///formula from system of equation, x = (b1 - b2) / (m2 - m1)
    float xPosition = (intercept - line.getIntercept() ) / (line.getSlope() - slope);

    //calculate the point of intersection
    intersectionPoint.x = xPosition;
    intersectionPoint.y = calculateOutput(xPosition);

    return intersectionExists(line, intersectionPoint);
}

bool LineSegment::intersectionExists(const LineSegment& line, const sf::Vector2f& intersectionPoint) const {

    //if point exists on both lines then its a valid intersection
    bool intersectionValid = (pointExists(intersectionPoint) && line.pointExists(intersectionPoint));

    return intersectionValid;
}

const float LineSegment::calculateOutput(const int input) const {

    return slope * input + intercept;
}
