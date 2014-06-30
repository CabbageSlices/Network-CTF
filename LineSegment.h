#ifndef LINESEGMENT_H_INCLUDED
#define LINESEGMENT_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

#include <tr1/memory>
#include <string>

//define a line with a given start and end point
//most of the times the line segment will only use the x coordinants of the start and end points in order to determine if a point is colliding with the line in the given domain
//however if the line has an undefined slope then it will use the y-component as the x component is non-existant, undefined slope lines should inherit from this class as they are a special case

///equation of line is y = mx + b; where m is the slope and b is the y-intercept
///derived classes may define b as something else

/**

    because it is difficult to calculate a point of intersection for two parallel lines (if they intersect they have to be on top of one another and that's
                                                                                         annoying to determine where the point of intersection lies)
    intersection between two parallel lines will not be calculated, meaning if two lines are parallel then intersection tests will always return false
**/

class LineSegment {

    private:

        sf::Vector2f startPoint;
        sf::Vector2f endPoint;

        float slope;

        //y intercept, derived classes can use this as the x-intercept instead
        int intercept;

        sf::VertexArray lineShape;

        //calculate y position using y=mx + b with a given x value
        const float calculateOutput(const int input) const;

        //texture for the line if user wants it to have graphics
        sf::Texture texture;

    protected:

        //don't allow user to create a line class themselves that way the line class can determine if it should be created
        //as a normal line or a line with an undefined slope
        //also load the given texture from the file with the texture coordinants defined, one texture coordinant for each vertex on the line
        ///factory method
        LineSegment(const sf::Vector2f& start, const sf::Vector2f& end, const float& lineSlope, const int& lineIntercept, const std::string& textureFile,
                    const sf::Vector2f& textureCoordinant1, const sf::Vector2f& texturecoordinant2);

        //check if the given point exists on the line, meaning it fits in the domain and range of line
        virtual bool pointExists(const sf::Vector2f& point) const;

        //check for intersection with a line with a slope that is parallel to this line
        //don't test right unless absolutely required, if parallel assume they don't intersect
        virtual bool intersectsParallelSlope(const LineSegment& line, sf::Vector2f& intersectionPoint) const;

        //check for intersection with a line with an undefined slope
        virtual bool intersectsUndefinedSlope(const LineSegment& line, sf::Vector2f& intersectionPoint) const;

        //check for intersection with a line that isn't a special case
        virtual bool intersectsNormalLine(const LineSegment& line, sf::Vector2f& intersectionPoint) const;

        //check if the given point of intersection exists on this line and the given line
        virtual bool intersectionExists(const LineSegment& line, const sf::Vector2f& intersectionPoint) const;

    public:

        virtual ~LineSegment();

        friend std::tr1::shared_ptr<LineSegment> createLine(const sf::Vector2f& start, const sf::Vector2f& end, const std::string& textureFile,
                    const sf::Vector2f& textureCoordinant1, const sf::Vector2f& texturecoordinant2);

        const float getSlope() const;
        const int getIntercept() const;

        const sf::Vector2f getStartPoint() const;
        const sf::Vector2f getEndPoint() const;

        ///assumes the new endpoint is still on the original line formed by this line, meaning the slope and intercepts are the same
        void setEndPoint(const sf::Vector2f& newEndPoint);

        //check if an intersection occurs between this line and the given line
        //inside behaves polymorphically
        //saves coordinatns of intersection into the vector given, if no intersection is found assume it's filled with garbage data
        ///parallel lines will always return false even if they intersect
        bool intersects(const LineSegment& line, sf::Vector2f& intersectionPoint) const;

        void draw(sf::RenderWindow& window);
};

//slope of a line with an undefined (infinite) slope
extern const float UNDEFINED_SLOPE;

//max size of a slope before it's considered an 'undefined' slope
extern const int MAX_SLOPE_SIZE;

//must call this function to create any line
std::tr1::shared_ptr<LineSegment> createLine(const sf::Vector2f& start, const sf::Vector2f& end, const std::string& textureFile,
                    const sf::Vector2f& textureCoordinant1, const sf::Vector2f& texturecoordinant2);

///wrapper for createline in order to give default values for texture data
std::tr1::shared_ptr<LineSegment> createLine(const sf::Vector2f& start, const sf::Vector2f& end);

//calculate the slope given two points
float calculateSlope(const sf::Vector2f& start, const sf::Vector2f& end);

float calculateIntercept(const sf::Vector2f& start, const float& slope);

#endif // LINESEGMENT_H_INCLUDED
