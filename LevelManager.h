#ifndef LEVELMANAGER_H_INCLUDED
#define LEVELMANAGER_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

#include <string>
#include <vector>
#include <tr1/memory>
#include <fstream>

class Block;
class ForegroundObject;

///tags to indcate what objects are being saved or loaded so you can easily search through files
extern const std::string BEGIN_BLOCKS;
extern const std::string END_BLOCKS;
extern const std::string BEGIN_FOREGROUND;
extern const std::string END_FOREGROUND;



///assumes the levelname is just the name of hte level without the file extention
bool saveLevel(const std::string& levelName, const std::vector<std::tr1::shared_ptr<Block> >& blocks,
               const std::vector<std::tr1::shared_ptr<ForegroundObject> >& objects);

bool loadLevel(const std::string& levelName, std::vector<std::tr1::shared_ptr<Block> >& blocks,
               std::vector<std::tr1::shared_ptr<ForegroundObject> >& objects);

//sets the stream position of the file directly past the lineToSet, meaning the lineToSet is ignored
//the given file must be able to perfom input operations
bool setFilePosition(const std::string& lineToSet, std::fstream& inputFile);

//returns the given string with the file extention added
const std::string addFileExtention(const std::string& fileName, const std::string& extention = ".txt");



//save the data of the given object to the given file
//save the given begin and end tags as the search tags
template <class Object>
bool saveObjects(const std::vector<std::tr1::shared_ptr<Object> >& objects, const std::string& beginTag, const std::string& endTag, std::fstream& file) {

    //put the blocks tag
    file << beginTag << std::endl;

    //now save all of the blocks
    for(auto& object : objects) {

        //only save the position of the block because the size of the block is set by the class itself
        sf::Vector2f position = object->getPosition();

        //save x position, then a line, then a y position
        file << position.x << std::endl;
        file << position.y << std::endl;
    }

    //indicate the end of the blocks
    file << endTag << std::endl;

    //check if the file is in an error state now
    if(!file.good()) {

        return false;
    }

    return true;
}

template <class Object>
bool loadObjects(const std::string& beginTag, const std::string& endTag, std::vector<std::tr1::shared_ptr<Object> >& objects, std::fstream& file) {

    //start reading from the beginning of the blocks by using the block tag
    if(!setFilePosition(beginTag, file)) {

        //failed to set position so blocksp robably not saved so exit
        return false;
    }

    //read lines until the end blocks tag is read, if its read then there are no more blocks to read or you reach the end of file
    std::string downloadedData;

    while(getline(file, downloadedData) && downloadedData != endTag) {

        //read the x and y position and create an object at the given position
        sf::Vector2f position(0, 0);

        //x position, x psotiion is read at the beginning of the loop in the condition
        position.x = atoi(downloadedData.c_str());

        //y position
        getline(file, downloadedData);
        position.y = atoi(downloadedData.c_str());

        //create the block
        std::tr1::shared_ptr<Object> object(new Object(position));

        objects.push_back(object);
    }

    //check if the stream is still in a valid state, if it isn't then reading failed
    if(!file.good()) {

        return false;
    }

    return true;
}

#endif // LEVELMANAGER_H_INCLUDED
