#include "Message.h"

using std::string;

sf::Time Message::lifeTime = sf::seconds(5);
sf::Texture Message::messages;
bool Message::loadedTexture = false;

const sf::IntRect Message::allySlainClip(19, 25, 203, 18);
const sf::IntRect Message::enemySlainClip(17, 7, 244, 18);
const sf::IntRect Message::playerDiedClip(18, 72, 179, 18);
const sf::IntRect Message::receivedKillClip(18, 54, 224, 18);

Message::Message(const sf::IntRect& clip):
    sprite(),
    timer()
    {
        //load consts if they haven't been loaded
        if(!loadedTexture) {

            messages.loadFromFile("images/messages.png");
            loadedTexture = true;
        }

        sprite.setTexture(messages);
        sprite.setTextureRect(clip);
    }
