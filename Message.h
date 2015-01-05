#ifndef MESSAGE_H_INCLUDED
#define MESSAGE_H_INCLUDED

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

#include <string>

///message that is displayed to the user to indicate different events
///the text for the message is predrawn
class Message {

    private:

        sf::Sprite sprite;
        sf::Clock timer;

        static sf::Time lifeTime;
        static sf::Texture messages;;
        static bool loadedTexture;

        void setupClips();

    public:

        static const sf::IntRect allySlainClip;
        static const sf::IntRect enemySlainClip;
        static const sf::IntRect playerDiedClip;
        static const sf::IntRect receivedKillClip;

        Message(const sf::IntRect& clip);

        void setPosition(float xPos, float yPos) {

            sprite.setPosition(xPos, yPos);
        }

        bool isActive() {

            return timer.getElapsedTime() < lifeTime;
        }

        void draw(sf::RenderWindow& window) {

            window.draw(sprite);
        }
};

#endif // MESSAGE_H_INCLUDED
