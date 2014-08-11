#include "Block.h"
#include "LevelManager.h"
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "Camera.h"
#include "ForegroundObject.h"

#include <vector>
#include <tr1/memory>
#include <iostream>

using std::vector;
using std::tr1::shared_ptr;
using std::cin;
using std::cout;
using std::endl;

int main() {

    sf::RenderWindow window(sf::VideoMode(1024, 768), "Level Editor");

    window.setKeyRepeatEnabled(false);

    sf::Event event;

    //container containing all of the blocks
    vector<shared_ptr<Block> > blocks;

    vector<shared_ptr<ForegroundObject> > foreground;

    bool creatingBlocks = false;
    bool destroyingBlocks = false;

    bool creatingForeground = false;
    bool destroyingForeground = false;

    //timer to set delay on how often a block is created or dstoryed while the button is held
    //that way user doens't have to spam click mouse to create a destory, unless they want to
    sf::Clock destoryBlockTimer;
    sf::Time destoryBlockDelay = sf::milliseconds(1);

    loadLevel("level", blocks, foreground);

    Camera camera;

    sf::Vector2f position(0, 0);
    sf::Vector2f levelSize(2000, 2000);

    camera.setDefaultSize(window);

    bool beginRect = false;
    sf::Vector2f rectOrigin(0, 0);
    sf::Vector2f rectEnd(0, 0);

    while(window.isOpen()) {

        while(window.pollEvent(event)) {

            if(event.type == sf::Event::Closed) {

                window.close();
            }

            if(event.type == sf::Event::KeyPressed) {

                if(event.key.code == sf::Keyboard::A) {

                    camera.zoomIn();
                }

                if(event.key.code == sf::Keyboard::S) {

                    camera.zoomOut();
                }

                if(event.key.code == sf::Keyboard::Z) {

                    creatingForeground = true;
                }

                if(event.key.code == sf::Keyboard::X) {

                    destroyingForeground = true;
                }
            }

            if(event.type == sf::Event::Resized) {

                camera.setDefaultSize(window);
            }

            if(event.type == sf::Event::MouseButtonPressed) {

                beginRect = true;

                rectOrigin = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            }

            if(event.type == sf::Event::MouseButtonReleased) {

                beginRect = false;


            }
        }

        if(beginRect) {

            rectEnd = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {

            position.y -= 1.5;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {

            position.y += 1.5;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {

            position.x -= 1.5;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {

            position.x += 1.5;
        }

        creatingBlocks = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        destroyingBlocks = sf::Mouse::isButtonPressed(sf::Mouse::Right);

        sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if(creatingBlocks) {

            shared_ptr<Block> newBlock(new Block(mousePosition));

            bool collides = false;

            for(auto block : blocks) {

                if(block->getCollisionBox().intersects(newBlock->getCollisionBox())) {

                    collides = true;
                    break;
                }
            }

            if(!collides) {

                blocks.push_back(newBlock);
            }
        }

        if(destroyingBlocks && destoryBlockTimer.getElapsedTime() > destoryBlockDelay) {

            //find the block in the given position and delete it, dlete the first one mouse finds
            for(unsigned int index = 0; index < blocks.size(); index++) {

                if(blocks[index]->getCollisionBox().contains(mousePosition)) {

                    blocks.erase(blocks.begin() + index);
                    break;
                }
            }

            destoryBlockTimer.restart();
        }

        if(creatingForeground) {

            shared_ptr<ForegroundObject> newForeground(new ForegroundObject(mousePosition));

            foreground.push_back(newForeground);
            creatingForeground = false;
        }

        if(destroyingForeground) {

            //find the block in the given position and delete it, dlete the first one mouse finds
            for(unsigned int index = 0; index < foreground.size(); index++) {

                if(foreground[index]->getCollisionBox().contains(mousePosition)) {

                    foreground.erase(foreground.begin() + index);
                    break;
                }
            }

            destroyingForeground = false;
        }

        if(position.x < 0) {

            position.x = 0;
        }

        if(position.x > levelSize.x) {

            position.x = levelSize.x;
        }

        if(position.y < 0) {

            position.y = 0;
        }

        if(position.y > levelSize.y) {

            position.y = levelSize.y;
        }

        camera.setCameraCenter(position, sf::FloatRect(0, 0, levelSize.x, levelSize.y));
        camera.applyCamera(window);

        sf::RectangleShape rect(sf::Vector2f(rectEnd.x - rectOrigin.x, rectEnd.y - rectOrigin.y));
        rect.setPosition(rectOrigin);

        cout << "x: " << rect.getPosition().x << " Rect width " << rect.getSize().x;
        cout << " global x: " << rect.getGlobalBounds().left << " bounds width " << rect.getGlobalBounds().width << endl;

        sf::RectangleShape coll(sf::Vector2f(-50, -50));
        coll.setPosition(500, 300);

        if(coll.getGlobalBounds().intersects(rect.getGlobalBounds())) {

            cout << "collision" << endl;
        }

        window.clear();

        window.draw(coll);
        window.draw(rect);

        window.display();
    }

    saveLevel("level", blocks, foreground);

    return 0;
}
