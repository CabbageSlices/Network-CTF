#include "SFML/Graphics.hpp"
#include "Input.h"

using std::string;

string exitedFunction = "~/*//@#$?<>";

string receiveInput(sf::RenderWindow& window, string message, const sf::Font& font)
{
    //ensure the previous window has no view attached so the inputs don't get screwed up
    window.setView(window.getDefaultView());

    //copy the current contents of the window that way the draw message can be drawn overtop of the window
    sf::Texture currentWindow;
    currentWindow.create(window.getSize().x, window.getSize().y);
    currentWindow.update(window);

    sf::Sprite currentWindowSprite;
    currentWindowSprite.setTexture(currentWindow);

    //constant for specific unicodes
    const unsigned unicodeBackspace = 8;
    const unsigned unicodeEnter = 13;
    const unsigned unicodeEscape = 27;

    string input;

    //load the font and create the text representing the input
    sf::Text inputText;
    inputText.setFont(font);

    const float textScale = 0.6;

    inputText.setScale(textScale, textScale);
    inputText.setColor(sf::Color::Blue);

    sf::Texture inputTexture;
    inputTexture.loadFromFile("inputMessage.png");

    //box to hold the message
    //its scaled by some random value that i chose that way there is more space to type
    sf::RectangleShape messageBox(sf::Vector2f(350, 100));
    messageBox.setTexture(&inputTexture);
    messageBox.setSize(sf::Vector2f(inputTexture.getSize().x, inputTexture.getSize().y));
    messageBox.scale(1.25, 1.25);

    //set the message box at the center of the screen
    messageBox.setPosition(window.getSize().x / 2 - messageBox.getSize().x / 2, window.getSize().y / 2 - messageBox.getSize().y / 2);

    ///set the max size of the input to ensure the input isn't too long, length in pixels so it's not a character limit
    const float maxInputLength = messageBox.getGlobalBounds().width * 0.8;

    //message asking user to input
    sf::Text inputMessage;
    inputMessage.setFont(font);
    inputMessage.setString(message);
    inputMessage.setScale(textScale, textScale);
    inputMessage.setPosition(messageBox.getGlobalBounds().left + messageBox.getGlobalBounds().width / 2 - inputMessage.getGlobalBounds().width / 2,
                             messageBox.getGlobalBounds().top + 10);

    sf::Event event;

    bool isActive = true;

    while(isActive)
    {
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                isActive = false;
                window.close();
            }

            if(event.type == sf::Event::TextEntered)
            {
                //take all valid text inputs and place them in the message
                if(event.text.unicode != unicodeBackspace && event.text.unicode != unicodeEnter && event.text.unicode != unicodeEscape)
                {
                    input += static_cast<char>(event.text.unicode);
                }else//backspace
                if(event.text.unicode == unicodeBackspace)
                {
                    if(input.size() > 0)
                    {
                        input.erase(input.end() - 1);
                    }
                }else
                if(event.text.unicode == unicodeEnter)
                {
                    isActive = false;
                }else
                if(event.text.unicode == unicodeEscape)
                {
                    isActive = false;
                    input = exitedFunction;
                }
            }
        }

        //set the message and place the text at the center of the message box
        inputText.setString(input);
        inputText.setPosition(messageBox.getGlobalBounds().left + messageBox.getGlobalBounds().width / 2 - inputText.getGlobalBounds().width / 2,
                              messageBox.getGlobalBounds().top + messageBox.getGlobalBounds().height - inputText.getGlobalBounds().height * 4);

        //if the size of the text exceeds the size of the message box then erase the last letter of the message so it fits
        while(inputText.getGlobalBounds().width > maxInputLength)
        {
            if(input.size() > 0)
            {
                input.erase(input.end() - 1);
                inputText.setString(input);
            }
        }

        window.clear();

        window.draw(currentWindowSprite);

        window.draw(messageBox);
        window.draw(inputMessage);
        window.draw(inputText);
        window.display();
    }

    return input;
}