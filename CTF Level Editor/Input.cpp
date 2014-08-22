#include "SFML/Graphics.hpp"
#include "Input.h"

using std::string;

const string CLOSED_PROGRAM = "-*//*/&(()$~";
const string EXITED_FUNCTION = "~/*//@#$?<>";

string receiveInput(sf::RenderWindow& window, const sf::Font& font)
{
    //ensure the previous window has no view attached so the inputs don't get screwed up
    //save previous view of window so you can restore it once function exits
    sf::View previousView = window.getView();
    window.setView(window.getDefaultView());

    //constant for specific unicodes
    const unsigned unicodeBackspace = 8;
    const unsigned unicodeEnter = 13;
    const unsigned unicodeEscape = 27;

    string input;

    //load the font and create the text
    sf::Text text;
    text.setFont(font);

    text.setCharacterSize(15);
    text.setColor(sf::Color::Black);

    sf::Texture inputTexture;
    inputTexture.loadFromFile("inputname.png");

    //box to hold the message
    sf::RectangleShape messageBox(sf::Vector2f(275, 100));
    messageBox.setTexture(&inputTexture);
    messageBox.setSize(sf::Vector2f(inputTexture.getSize().x, inputTexture.getSize().y));

    //set the message box at the center of the screen
    messageBox.setPosition(window.getView().getSize().x / 2 - messageBox.getSize().x / 2, window.getView().getSize().y / 2 - messageBox.getSize().y / 2);

    ///set the max size of the name to ensure the name isn't too long, length in pixels so it's not a character limit
    const float maxInputLength = 360;

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
                input = CLOSED_PROGRAM;
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
                    input = EXITED_FUNCTION;
                }
            }
        }

        //set the message and place the text at the center of the message box
        text.setString(input);
        text.setPosition(messageBox.getPosition().x + messageBox.getSize().x / 2 - text.getGlobalBounds().width / 2, messageBox.getPosition().y + messageBox.getSize().y / 2 - text.getGlobalBounds().height / 2);

        //if the size of the text exceeds the size of the message box then erase the last letter of the message so it fits
        while(text.getGlobalBounds().width > maxInputLength)
        {
            if(input.size() > 0)
            {
                input.erase(input.end() - 1);
                text.setString(input);
            }
        }

        window.draw(messageBox);
        window.draw(text);
        window.display();
    }

    //restore previous view of window
    window.setView(previousView);

    return input;
}
