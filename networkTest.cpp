#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "ClientGameManager.h"
#include "ServerGameManager.h"
#include "Input.h"
#include "PredrawnButton.h"
#include "ButtonPlacer.h"
#include "TypeChecker.h"

#include <tr1/memory>
#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>

using std::vector;
using std::tr1::shared_ptr;
using std::cout;
using std::endl;
using std::string;
using std::atoi;

void clientTitleScreen(sf::RenderWindow& window);
void findMatchScreen(sf::RenderWindow& window);

void serverTitleScreen(sf::RenderWindow& window);

int main() {

    srand(static_cast<long>(time(0) ));

    sf::Vector2f screenSize(1024, 768);

    //disable the resizing in the window at the beginning because the title screen and such have a fixed size
    sf::RenderWindow window(sf::VideoMode(screenSize.x, screenSize.y), "Network Test", sf::Style::Titlebar | sf::Style::Close);
    window.setKeyRepeatEnabled(false);

    ///clientTitleScreen(window);
    serverTitleScreen(window);

    ServerGameManager serverGameManager;

    sf::View largeView;
    largeView.setViewport(sf::FloatRect(0, 0, 1, 1));
    largeView.setSize(3200, 1800);
    window.setView(largeView);

    ///serverGameManager.gameLobby(window);
    ///serverGameManager.runGame(window);

    return 0;
}

void clientTitleScreen(sf::RenderWindow& window) {

    //load up the image of the title screen
    sf::Texture titleScreenTexture;
    titleScreenTexture.loadFromFile("clientTitleScreen.png");

    sf::Sprite titleScreenSprite;
    titleScreenSprite.setTexture(titleScreenTexture);

    //buttons that can be pressed
    sf::FloatRect findMatchButton(206, 132, 287, 62);

    sf::Event event;

    while(window.isOpen()) {

        while(window.pollEvent(event)) {

            if(event.type == sf::Event::Closed) {

                window.close();
            }

            if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {

                sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if(findMatchButton.contains(mousePosition)) {

                    findMatchScreen(window);
                }
            }
        }

        window.clear();

        window.draw(titleScreenSprite);

        window.display();
    }
}

void findMatchScreen(sf::RenderWindow& window) {

    //load the images and buttons required for the find match screen
    sf::Texture findMatchTexture;
    findMatchTexture.loadFromFile("findmatchscreen.png");

    sf::Sprite findMatchSprite;
    findMatchSprite.setTexture(findMatchTexture);

    sf::FloatRect serverIpBox(247, 142, 207, 32);
    sf::FloatRect serverPortBox(246, 207, 211, 35);
    sf::FloatRect nameBox(243, 105, 216, 23);
    sf::FloatRect backButton(490, 29, 112, 83);
    sf::FloatRect connectBox(163, 299, 296, 67);

    sf::Font font;
    font.loadFromFile("font.ttf");

    const float textScale = 0.6;

    //create a text representation of the current server ip and port the user is trying to connect to
    sf::Text serverIp;
    serverIp.setString("70.71.114.74");
    serverIp.setFont(font);
    serverIp.setPosition(serverIpBox.left, serverIpBox.top);
    serverIp.setColor(sf::Color::Blue);
    serverIp.setScale(textScale, textScale);

    sf::Text serverPort;
    serverPort.setString("8080");
    serverPort.setFont(font);
    serverPort.setPosition(serverPortBox.left, serverPortBox.top);
    serverPort.setColor(sf::Color::Blue);
    serverPort.setScale(textScale, textScale);

    sf::Text playerName;
    playerName.setString("Name");
    playerName.setFont(font);
    playerName.setPosition(nameBox.left, nameBox.top);
    playerName.setColor(sf::Color::Blue);
    playerName.setScale(textScale, textScale);

    sf::Event event;

    while(window.isOpen()) {

        while(window.pollEvent(event)) {

            if(event.type == sf::Event::Closed) {

                window.close();
            }

            if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {

                sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if(serverIpBox.contains(mousePosition)) {

                    string ipToConnectTo = receiveInput(window, "Enter the server's Ip Address", font);
                    serverIp.setString(ipToConnectTo);

                } else if(serverPortBox.contains(mousePosition)) {

                    string portToConnectTo = receiveInput(window, "Enter the server's port", font);
                    serverPort.setString(portToConnectTo);

                } else if(nameBox.contains(mousePosition)) {

                    string name = receiveInput(window, "Enter your player name", font);

                    if(name != "") {

                        playerName.setString(name);
                    }

                } else if(connectBox.contains(mousePosition)) {

                    ClientGameManager client;
                    client.setPlayerName(playerName.getString());

                    string ipAddress = serverIp.getString();
                    unsigned short port = static_cast<unsigned short>(atoi(serverPort.getString().toAnsiString().c_str()) );

                    if(client.connectToServer(ipAddress, port)) {

                        client.gameLobby(window, font);
                    }
                }

                else if(backButton.contains(mousePosition)) {

                    return;
                }
            }
        }

        window.clear();

        window.draw(findMatchSprite);
        window.draw(serverIp);
        window.draw(serverPort);
        window.draw(playerName);

        window.display();
    }
}

void serverTitleScreen(sf::RenderWindow& window) {

    sf::Texture titleScreenTexture;
    sf::Sprite titleScreenSprite;

    titleScreenTexture.loadFromFile("serverTitleScreen.png");
    titleScreenSprite.setTexture(titleScreenTexture);

    //create buttons for user to press
    vector<shared_ptr<PredrawnButton> > buttons;

    //id for each button that is in the vector that way its easy to access
    unsigned hostGameId = 0;

    //create all buttons
    buttons.push_back(shared_ptr<PredrawnButton>(new PredrawnButton("hostGameButton.png")) );

    placeButtons<shared_ptr<PredrawnButton> >("serverTitlescreen.png" , buttons);

    //create a button for user to press in order to change port numbers
    sf::FloatRect changePortButton(262, 119, 215, 42);

    sf::Font font;
    font.loadFromFile("font.ttf");

    //text to save the server port
    sf::Text serverPortText;
    serverPortText.setFont(font);
    serverPortText.setScale(0.6, 0.6);
    serverPortText.setString("8080");
    serverPortText.setPosition(273, 126);
    serverPortText.setColor(sf::Color::Red);

    sf::Event event;

    ServerGameManager server;

    while(window.isOpen()) {

        while(window.pollEvent(event)) {

            if(event.type == sf::Event::Closed) {

                window.close();
            }

            if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {

                sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                //don't attempt to host a game if port isn't even a number
                if(buttons[hostGameId]->checkMouseTouching(mousePosition) && isDigit(serverPortText.getString().toAnsiString())) {

                    unsigned port = atoi(serverPortText.getString().toAnsiString().c_str());

                    //attempt to bind to the socket, if failed then let user know
                    if(server.bindToPort(port)) {

                        server.gameLobby(window);

                    } else {

                        ///do something to indicate a failure
                    }
                }

                if(changePortButton.contains(mousePosition)) {

                    string port = receiveInput(window, "Enter the host port.", font);

                    if(port.size() > 0) {

                        serverPortText.setString(port);
                    }
                }
            }
        }

        sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getDefaultView());

        for(auto& button : buttons) {

            button->checkMouseTouching(mousePosition);
        }

        window.clear();

        window.draw(titleScreenSprite);

        for(auto& button : buttons) {

            button->draw(window);
        }

        window.draw(serverPortText);

        window.display();
    }
}
