#ifndef LEVELMANAGER_H_INCLUDED
#define LEVELMANAGER_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

#include <string>
#include <vector>
#include <tr1/memory>
#include <fstream>

#include "Floor.h"
#include "Flag.h"

class LargeImage;
class Block;
class ForegroundObject;
class Portal;
class GunGiver;

///tags to indcate what objects are being saved or loaded so you can easily search through files
extern const std::string BEGIN_BLOCKS;
extern const std::string END_BLOCKS;
extern const std::string BEGIN_FOREGROUND;
extern const std::string END_FOREGROUND;
extern const std::string BEGIN_PORTAL;
extern const std::string END_PORTAL;
extern const std::string BEGIN_GUN_GIVER;
extern const std::string END_GUN_GIVER;
extern const std::string BEGIN_RED_FLAG;
extern const std::string END_RED_FLAG;
extern const std::string BEGIN_BLUE_FLAG;
extern const std::string END_BLUE_FLAG;
extern const std::string BEGIN_RED_SPAWN;
extern const std::string END_RED_SPAWN;
extern const std::string BEGIN_BLUE_SPAWN;
extern const std::string END_BLUE_SPAWN;
extern const std::string BEGIN_OVERGROUND_IMAGE;
extern const std::string END_OVERGROUND_IMAGE;
extern const std::string BEGIN_UNDERGROUND_IMAGE;
extern const std::string END_UNDERGROUND_IMAGE;


///assumes the levelname is just the name of the level without the file extention
bool saveOvergroundLevel(const std::string& levelName, std::tr1::shared_ptr<Floor>& floor, Flag& redFlag, Flag& blueFlag,
               const sf::FloatRect& redSpawn, const sf::FloatRect& blueSpawn);

///assumes levelname is just the name of the level without the file extention
bool saveUndergroundLevel(const std::string& levelName, std::tr1::shared_ptr<Floor>& floor);

bool loadOvergroundLevel(const std::string& levelName, std::tr1::shared_ptr<Floor>& floor, Flag& redFlag, Flag& blueFlag,
                         sf::FloatRect& redSpawn, sf::FloatRect& blueSpawn);

bool loadUndergroundLevel(const std::string& levelName, std::tr1::shared_ptr<Floor>& floor);

//sets the stream position of the file directly past the lineToSet, meaning the lineToSet is ignored
//the given file must be able to perfom input operations
bool setFilePosition(const std::string& lineToSet, std::fstream& inputFile);

//returns the given string with the file extention added
const std::string addFileExtention(const std::string& fileName, const std::string& extention = ".txt");

//save the given floor
//when it saves the background it doesn't know if its the overground floor's background or the underground floor's background
//so you must specify the appropriate tags for the background
bool saveFloor(std::fstream& file, std::tr1::shared_ptr<Floor>& floor, const std::string& backgroundBegin, const std::string& backgroundEnd);

bool saveFlag(std::fstream& file, Flag& flag, const std::string& beginTag, const std::string& endTag);
bool saveSpawn(std::fstream& file, const sf::FloatRect& spawnArea, const std::string& beginTag, const std::string& endTag);
bool saveBackground(std::fstream& file, const LargeImage& image, const std::string& beginTag, const std::string& endTag);

bool loadFloor(std::fstream& file, std::tr1::shared_ptr<Floor>& floor, const std::string& backgroundBegin, const std::string& backgroundEnd);

bool loadFlag(std::fstream& file, Flag& flag, const std::string& beginTag, const std::string& endTag);
bool loadSpawn(std::fstream& file, sf::FloatRect& spawnArea, const std::string& beginTag, const std::string& endTag);
bool loadBackground(std::fstream& file, LargeImage& image, const std::string& beginTag, const std::string& endTag);

//functions are passed as arguments to the saveObjects and loadObjcts functions
void saveBlock(std::fstream& file, const std::tr1::shared_ptr<Block>& block);
void saveForeground(std::fstream& file, const std::tr1::shared_ptr<ForegroundObject>& foregroundObj);
void savePortal(std::fstream& file, const std::tr1::shared_ptr<Portal>& portal);
void saveGunGiver(std::fstream& file, const std::tr1::shared_ptr<GunGiver>& gunGiver);

std::tr1::shared_ptr<Block> loadBlock(std::fstream& file, const std::string& endTag);
std::tr1::shared_ptr<ForegroundObject> loadForeground(std::fstream& file, const std::string& endTag);
std::tr1::shared_ptr<Portal> loadPortal(std::fstream& file, const std::string& endTag);
std::tr1::shared_ptr<GunGiver> loadGunGiver(std::fstream& file, const std::string& endTag);

//save the data of the given object to the given file
//save the given begin and end tags as the search tags
//use the given function to extract the data used to save the object
///saveobjects needs to take a file as a parameter because if this function opens the file object itself then it won't know whether to clear the previous contents of the file
///or to just append to the end of the file
template <class Object>
bool saveObjects(const std::vector<std::tr1::shared_ptr<Object> >& objects, const std::string& beginTag, const std::string& endTag, std::fstream& file,
                 void (*saveFunction)(std::fstream& file, const std::tr1::shared_ptr<Object>& obj)) {

    //put the blocks tag
    file << beginTag << std::endl;

    //now save all of the blocks
    for(auto object : objects) {

        (*saveFunction)(file, object);
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
bool loadObjects(std::vector<std::tr1::shared_ptr<Object> >& objects, const std::string& beginTag, const std::string& endTag, std::fstream& file,
                 std::tr1::shared_ptr<Object> (*createObject)(std::fstream& file, const std::string& endTag)) {

    //start reading from the beginning of the objects by using the block tag
    if(!setFilePosition(beginTag, file)) {

        //failed to set position so object probably not saved so exit
        return false;
    }

    //function given will read lines until there are no more objects of this type left to create
    //if it returns a null pointer it means it reached the end tag
    while(true) {

        std::tr1::shared_ptr<Object> loadedObject = (*createObject)(file, endTag);

        //object was not loaded, meaning end tag was found or some other problem so stop loading
        if(!loadedObject) {

            break;
        }

        //object loaded, put into container
        objects.push_back(loadedObject);
    }

    //check if the stream is still in a valid state, if it isn't then reading failed
    if(!file.good()) {

        return false;
    }

    return true;
}

#endif // LEVELMANAGER_H_INCLUDED
