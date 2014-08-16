#ifndef LARGEIMAGE_H_INCLUDED
#define LARGEIMAGE_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

#include <vector>
#include <tr1/memory>
#include <string>

//because sfml textures have a restriction on their size according to graphics card
//this class is used to load images and split them into smaller textures that fit the graphics card size
class LargeImage {

    private:

        sf::Image image;

        std::vector<std::tr1::shared_ptr<sf::Texture> > textures;
        std::vector<std::tr1::shared_ptr<sf::Sprite> > sprites;

        //splits the large image into smaller chunks whose dimensions are <= the maximum texture size for the computer
        void createPeices();

    public:

        LargeImage();

        bool load(const std::string& imageName);
        void clearImage();

        void draw(sf::RenderWindow& window);
};

#endif // LARGEIMAGE_H_INCLUDED
