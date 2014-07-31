#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

int main() {

    sf::RenderWindow window(sf::VideoMode(1024, 768), "Test");

    sf::Event event;

    sf::RectangleShape rect(sf::Vector2f(200, 200));

    sf::View view;
    view.setSize(1024, 768);

    sf::RenderTexture texture;

    texture.create(725, 684);
    texture.clear(sf::Color::Red);
    texture.draw(rect);
    texture.display();

    sf::Sprite sprite(texture.getTexture());

    while(window.isOpen()) {

        while(window.pollEvent(event)) {

            if(event.type == sf::Event::Closed) {

                window.close();
            }
        }

        window.clear();

        window.draw(sprite);
        window.display();
    }
}
