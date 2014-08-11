#ifndef STATICOBJECT_H_INCLUDED
#define STATICOBJECT_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

//static objects are non moving objects such as blocks or trees
class StaticObject {

    protected:

        sf::RectangleShape collisionBox;

        virtual void setupCollisionBox();

    public:

        StaticObject(const sf::Vector2f& centerPosition);
        virtual ~StaticObject();

        const sf::FloatRect getCollisionBox() const;
        const sf::Vector2f getPosition() const;


        virtual void draw(sf::RenderWindow& window);
};

#endif // STATICOBJECT_H_INCLUDED
