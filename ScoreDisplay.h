#ifndef SCOREDISPLAY_H_INCLUDED
#define SCOREDISPLAY_H_INCLUDED

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

class ScoreDisplay {

    private:

        sf::Texture scoreTexture;
        sf::Sprite scoreSprite;

        //offset where scores of each team are displayed
        sf::Vector2f RED_SCORE_OFFSET;
        sf::Vector2f BLUE_SCORE_OFFSET;

        sf::Font font;

        const float TEXT_SCALE;

        sf::Text redScore;
        sf::Text blueScore;

        //position the entire score display
        void positionDisplay(const sf::Vector2u& screenSize);

        //position the text representing the teams score
        void positionScores();

    public:

        ScoreDisplay(const sf::Vector2u& screenSize);

        void handleScreenResize(const sf::Vector2u& screenSize);

        void setRedScore(unsigned short& score);
        void setBlueScore(unsigned short& score);

        void draw(sf::RenderWindow& window);
};

#endif // SCOREDISPLAY_H_INCLUDED
