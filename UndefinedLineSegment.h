#ifndef UNDEFINEDLINESEGMENT_H_INCLUDED
#define UNDEFINEDLINESEGMENT_H_INCLUDED

#include "LineSegment.h"

class UndefinedLineSegment : public LineSegment {

    protected:

        UndefinedLineSegment(const sf::Vector2f& start, const sf::Vector2f& end, const int& lineIntercept, const std::string& textureFile,
                    const sf::Vector2f& textureCoordinant1, const sf::Vector2f& texturecoordinant2);

        //check if the given point exists on the line, meaning it fits in the domain and range of line
        virtual bool pointExists(const sf::Vector2f& point) const;

        //check for intersection with a line with a slope that is parallel to this line
        ///don't test right unless absolutely required, if parallel assume they don't intersect
        virtual bool intersectsParallelSlope(const LineSegment& line, sf::Vector2f& intersectionPoint) const;

        //check for intersection with a line with an undefined slope
        virtual bool intersectsUndefinedSlope(const LineSegment& line, sf::Vector2f& intersectionPoint) const;

        //check for intersection with a line that isn't a special case
        virtual bool intersectsNormalLine(const LineSegment& line, sf::Vector2f& intersectionPoint) const;

    public:

        friend std::tr1::shared_ptr<LineSegment> createLine(const sf::Vector2f& start, const sf::Vector2f& end, const std::string& textureFile,
                    const sf::Vector2f& textureCoordinant1, const sf::Vector2f& texturecoordinant2);

        virtual ~UndefinedLineSegment();
};

std::tr1::shared_ptr<LineSegment> createLine(const sf::Vector2f& start, const sf::Vector2f& end, const std::string& textureFile,
                    const sf::Vector2f& textureCoordinant1, const sf::Vector2f& texturecoordinant2);

#endif // UNDEFINEDLINESEGMENT_H_INCLUDED
