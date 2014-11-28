#ifndef STATICOBJECT_H_INCLUDED
#define STATICOBJECT_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

class UserPlayer;

//static objects are non moving objects such as blocks or trees
class StaticObject {

    protected:

        sf::RectangleShape collisionBox;

        virtual void setupCollisionBox();

    public:

        StaticObject(const sf::Vector2f& position);
        virtual ~StaticObject();

        const sf::FloatRect getCollisionBox() const;
        const sf::Vector2f getPosition() const;

        bool contains(const sf::Vector2f& point) const;

        virtual void handleCollision(UserPlayer& collidingPlayer);

        virtual void draw(sf::RenderWindow& window);
};

#endif // STATICOBJECT_H_INCLUDED
