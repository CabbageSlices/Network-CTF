#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "ClientGameManager.h"
#include "ServerGameManager.h"
#include <iostream>

using std::cout;
using std::endl;

int main() {

    srand(static_cast<long>(time(0) ));

    sf::Vector2f screenSize(1024, 768);

    sf::RenderWindow window(sf::VideoMode(screenSize.x, screenSize.y), "Network Test");

    ClientGameManager gameManager;

    window.setKeyRepeatEnabled(false);

    gameManager.runGame(window);

    ServerGameManager serverGameManager;

    ///serverGameManager.runGame(window);

    return 0;
}
