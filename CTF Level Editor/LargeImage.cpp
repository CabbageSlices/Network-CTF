#include "LargeImage.h"
#include <cmath>

using std::ceil;
using std::string;
using std::tr1::shared_ptr;

LargeImage::LargeImage() :
    image(),
    textures(),
    sprites(),
    loadedImage()
    {

    }

bool LargeImage::load(const string& fileName) {

    if(!image.loadFromFile(fileName)) {

        return false;
    }

    //loading new image so clear previous ones
    clearImage();

    //image has been loaded successfully so split the image into smaller textures for drawing
    createPeices();

    loadedImage = fileName;

    return true;
}

void LargeImage::clearImage() {

    //clear the saved image and all textures and sprites
    textures.clear();
    sprites.clear();

    loadedImage = "";
}

const sf::Vector2f LargeImage::getSize() const {

    return sf::Vector2f(image.getSize().x, image.getSize().y);
}

const std::string& LargeImage::getImagePath() const {

    return loadedImage;
}

void LargeImage::draw(sf::RenderWindow& window) {

    for(auto& sprite : sprites) {

        window.draw(*sprite);
    }
}

void LargeImage::createPeices() {

    const unsigned int maxTextureSize = sf::Texture::getMaximumSize();

    //calculate the number peices you need to split the large image into
    //on the x axis and the y axis because the two axis's have different sizes and different size limitations
    //round the amount up because if the image can be divided into 2.5 pecise, you need 3 textures to completely encapusalte the entire image
    int xPeices = ceil(static_cast<float>(image.getSize().x) / static_cast<float>(maxTextureSize) );
    int yPeices = ceil(static_cast<float>(image.getSize().y) / static_cast<float>(maxTextureSize) );

    //the image will be split into a multidimesional array, however we are storing it in a single dimensional vector isntead
    for(int yPeicesCreated = 0; yPeicesCreated < yPeices; ++yPeicesCreated) {

        for(int xPeicesCreated = 0; xPeicesCreated < xPeices; ++xPeicesCreated) {

            //calculate the area of the texture, you don't need to worry about the area exceeding the image bounds because sf::texture automatically fits the bounds
            sf::IntRect area(xPeicesCreated * maxTextureSize,
                             yPeicesCreated * maxTextureSize,
                             maxTextureSize,
                             maxTextureSize);

            shared_ptr<sf::Texture> newTexture(new sf::Texture());

            //texture successfuly created so save it, discarded otherwise
            if(newTexture->loadFromImage(image, area)) {

                //now create the sprite from the texture
                shared_ptr<sf::Sprite> newSprite(new sf::Sprite());
                newSprite->setTexture(*newTexture, true);

                //by setting the origin to this position you can move the image around just usting the set position function instead of calculating the new position each time
                newSprite->setOrigin(-area.left, -area.top);

                textures.push_back(newTexture);
                sprites.push_back(newSprite);
            }
        }
    }
}
