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

        //save the scores as numbers as well that way you can easily access them
        unsigned redScoreNum;
        unsigned blueScoreNum;

        //messages to display to player about either teams flags
        sf::Texture messageTexture;

        sf::Sprite allyStoleFlag;
        sf::Sprite allyCapturedFlag;

        sf::Sprite enemyStoleFlag;
        sf::Sprite enemyCapturedFlag;

        //how long to draw the capture messages for
        sf::Clock allyCaptureTimer;

        sf::Clock enemyCaptureTimer;

        //how long a capture message is displayed
        sf::Time captureDisplayTime;

        //whether a flag stolen message should be drawn
        bool drawAllyStolen;
        bool drawEnemyStolen;

        //position the entire score display
        void positionDisplay(const sf::Vector2u& screenSize);

        //position the text representing the teams score
        void positionScores();

        void initMessages();

    public:

        ScoreDisplay(const sf::Vector2u& screenSize);

        void handleScreenResize(const sf::Vector2u& screenSize);

        void setRedScore(unsigned short& score);
        void setBlueScore(unsigned short& score);

        const unsigned& getRedScore() {

            return redScoreNum;
        }

        const unsigned& getBlueScore() {

            return blueScoreNum;
        }

        void setAllyStolen(bool stolen) {

            drawAllyStolen = stolen;
        }

        void setEnemyStolen(bool stolen) {

            drawEnemyStolen = stolen;
        }

        void drawAllyCapture() {

            allyCaptureTimer.restart();
            drawAllyStolen = false;
        }

        void drawEnemyCapture() {

            enemyCaptureTimer.restart();
            drawEnemyStolen = false;
        }

        void draw(sf::RenderWindow& window);

        void resetDisplay() {

            redScoreNum = 0;
            blueScoreNum = 0;

            drawAllyStolen = false;
            drawEnemyStolen = false;
        }
};

#endif // SCOREDISPLAY_H_INCLUDED
