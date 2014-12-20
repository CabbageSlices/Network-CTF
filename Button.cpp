#include "Button.h"
#include <iostream>

using std::cout;
using std::endl;
using std::string;
using std::function;

Button::Button(const string& imagePath, const string& fontPath, const string& message, const sf::Vector2f& imageBorders):
    texture(),
    sprite(),
    font(),
    borders(imageBorders),
    text(),
    cover(sf::Vector2f(1, 1)),
    isMouseTouching(false)
    {
        setup(imagePath, fontPath, message);
    }

void Button::setPosition(const sf::Vector2f& position) {

    text.setPosition(position.x - text.getGlobalBounds().width / 2, position.y - text.getGlobalBounds().height / 2);

    repositionSprite();
}

void Button::setText(const string& message) {

    //get the current center position because after changing the text it will no longer be centered
    //so save the current center position and reset to center at the end
    sf::Vector2f currentCenter(text.getPosition().x + text.getGlobalBounds().width / 2,
                               text.getPosition().y + text.getGlobalBounds().height / 2);

    text.setString(message);
    resizeSprite();

    setPosition(currentCenter);
}

void Button::setScale(sf::Vector2f scaleValue) {

    doScale(scaleValue, [this](const sf::Vector2f& scaleValue)
            {text.setScale(scaleValue);}
            );
}

void Button::scale(sf::Vector2f scaleValue) {

    doScale(scaleValue, [this](const sf::Vector2f& scaleValue)
            {text.scale(scaleValue);}
            );
}

bool Button::checkMouseTouching(sf::Vector2f& mousePosition) {

    isMouseTouching = sprite.getGlobalBounds().contains(mousePosition);

    return isMouseTouching;
}

void Button::draw(sf::RenderWindow& window) {

    window.draw(sprite);
    window.draw(text);

    if(isMouseTouching) {

        window.draw(cover);
    }
}

void Button::setup(const string& imagePath, const string& fontPath, const string& message) {

    texture.loadFromFile(imagePath);
    font.loadFromFile(fontPath);

    sprite.setTexture(texture);
    text.setFont(font);
    text.setString(message);

    cover.setFillColor(sf::Color(0, 0, 0, 100));

    resizeSprite();
    setPosition(sf::Vector2f(0, 0));
}

void Button::resizeSprite() {

    //resize the sprite so that the text can fit completely inside

    //size of the text with the border
    float textBorderWidth = text.getGlobalBounds().width + borders.x * 2;
    float textBorderHeight = text.getGlobalBounds().height + borders.y * 2;

    float xScaling = textBorderWidth / sprite.getGlobalBounds().width;
    float yScaling = textBorderHeight / sprite.getGlobalBounds().height;

    sprite.scale(xScaling, yScaling);
    cover.setSize(sf::Vector2f(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height));
}

void Button::repositionSprite() {

    //set the center of the sprite to the center of the text
    sprite.setPosition(text.getGlobalBounds().left + text.getGlobalBounds().width / 2 - sprite.getGlobalBounds().width / 2,
                       text.getGlobalBounds().top + text.getGlobalBounds().height / 2 - sprite.getGlobalBounds().height / 2);

    cover.setPosition(sprite.getGlobalBounds().left, sprite.getGlobalBounds().top);
}

void Button::doScale(const sf::Vector2f& scaleValue, function<void(const sf::Vector2f& scaleValue)> scaleFunction) {

    //get the current center position since the text scales from the top left and not from the center
    //so it will no longer be centered
    sf::Vector2f currentCenter(text.getPosition().x + text.getGlobalBounds().width / 2,
                               text.getPosition().y + text.getGlobalBounds().height / 2);

    //scale the text
    scaleFunction(scaleValue);

    //border is also scaled
    borders.x *= scaleValue.x;
    borders.y *= scaleValue.y;

    resizeSprite();

    //now set the position of the text to the previous center
    setPosition(currentCenter);
}
