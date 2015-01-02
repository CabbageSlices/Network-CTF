#include "ErrorMessage.h"
#include "PredrawnButton.h"
#include "ButtonPlacer.h"

#include <vector>
#include <tr1/memory>

using std::tr1::shared_ptr;
using std::string;
using std::vector;

void displayError(sf::RenderWindow& window, const string& message) {

    //save the view of the previous screen because you have to set to default view in order to draw all components of this screen
    sf::View previousView = window.getView();
    window.setView(window.getDefaultView());

    //save the previous screen so you can display the error message overtop of the previous screen
    sf::Texture previousScreenTexture;
    previousScreenTexture.create(window.getSize().x, window.getSize().y);
    previousScreenTexture.update(window);

    sf::Sprite previousScreenSprite;
    previousScreenSprite.setTexture(previousScreenTexture);

    //load up the texture for the error message screen
    sf::Texture errorScreen;
    errorScreen.loadFromFile("errorMessage.png");

    sf::Sprite errorSprite;
    errorSprite.setTexture(errorScreen);

    //create the text representation of the message
    sf::Font font;
    font.loadFromFile("font.ttf");

    //the position is based on the image file of the error message so look to that if you want to know why the position was chosen
    sf::Text errorMessage;
    errorMessage.setFont(font);
    errorMessage.setString(message);
    errorMessage.setScale(0.41, 0.45);
    errorMessage.setPosition(363, 292);
    errorMessage.setColor(sf::Color::Black);

    //button to return to previous screen
    //put in container to use button placer function
    vector<shared_ptr<PredrawnButton> > buttons;
    buttons.push_back(shared_ptr<PredrawnButton>(new PredrawnButton("backButton.png") ));

    placeButtons("errormessage.png", buttons);

    sf::SoundBuffer errorBuffer;
    errorBuffer.loadFromFile("sounds/errorSound.wav");

    sf::Sound errorSound;
    errorSound.setBuffer(errorBuffer);
    errorSound.play();

    //index of buttons
    unsigned okButton = 0;

    sf::Event event;

    while(window.isOpen()) {

        while(window.pollEvent(event)) {

            if(event.type == sf::Event::Closed) {

                window.close();
            }

            if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {

                PredrawnButton::playClickSound();
                sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getDefaultView());

                if(buttons[okButton]->checkMouseTouching(mousePosition)) {

                    window.setView(previousView);
                    return;
                }
            }
        }

        sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getDefaultView());

        for(auto& button : buttons) {

            button->checkMouseTouching(mousePosition);
        }

        window.clear();

        window.draw(previousScreenSprite);

        window.draw(errorSprite);

        for(auto& button : buttons) {

            button->draw(window);
        }

        window.draw(errorMessage);

        window.display();
    }

    window.setView(previousView);
}
