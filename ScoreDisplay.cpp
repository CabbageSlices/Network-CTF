#include "ScoreDisplay.h"
#include "Conversion.h"

ScoreDisplay::ScoreDisplay(const sf::Vector2u& screenSize):
    scoreTexture(),
    scoreSprite(),
    RED_SCORE_OFFSET(125, 88),
    BLUE_SCORE_OFFSET(163, 88),
    font(),
    TEXT_SCALE(0.35),
    redScore(),
    blueScore()
    {
        scoreTexture.loadFromFile("scoredisplay.png");
        scoreSprite.setTexture(scoreTexture);

        font.loadFromFile("font.ttf");

        redScore.setFont(font);
        redScore.setScale(TEXT_SCALE, TEXT_SCALE);

        blueScore.setFont(font);
        blueScore.setScale(TEXT_SCALE, TEXT_SCALE);

        positionDisplay(screenSize);
    }

void ScoreDisplay::handleScreenResize(const sf::Vector2u& screenSize) {

    positionDisplay(screenSize);
}

void ScoreDisplay::setRedScore(unsigned short& score) {

    redScore.setString(toString(score));
}

void ScoreDisplay::setBlueScore(unsigned short& score) {

    blueScore.setString(toString(score));
}

void ScoreDisplay::draw(sf::RenderWindow& window) {

    window.draw(scoreSprite);
    window.draw(redScore);
    window.draw(blueScore);
}

void ScoreDisplay::positionDisplay(const sf::Vector2u& screenSize) {

    scoreSprite.setPosition(screenSize.x / 2 - scoreSprite.getGlobalBounds().width / 2, 0);

    positionScores();
}

void ScoreDisplay::positionScores() {

    redScore.setPosition(scoreSprite.getPosition().x + RED_SCORE_OFFSET.x,
                         scoreSprite.getPosition().y + RED_SCORE_OFFSET.y);


    blueScore.setPosition(scoreSprite.getPosition().x + BLUE_SCORE_OFFSET.x,
                          scoreSprite.getPosition().y + BLUE_SCORE_OFFSET.y);

}
