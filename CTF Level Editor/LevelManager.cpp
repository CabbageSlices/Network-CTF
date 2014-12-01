#include "LevelManager.h"
#include "Block.h"
#include "ForegroundObject.h"
#include "Portal.h"
#include "GunGiver.h"
#include "GunTypes.h"
#include "LargeImage.h"
#include "StaticObject.h"

#include <iostream>
#include <cstdlib>

using std::vector;
using std::tr1::shared_ptr;
using std::string;
using std::fstream;
using std::cout;
using std::endl;
using std::getline;
using std::atof;

const std::string BEGIN_BLOCKS = "start blocks";
const std::string END_BLOCKS = "end blocks";
const std::string BEGIN_FOREGROUND = "start foreground";
const std::string END_FOREGROUND = "end foreground";
const std::string BEGIN_PORTAL = "begin portal";
const std::string END_PORTAL = "end portal";
const std::string BEGIN_GUN_GIVER = "begin gun giver";
const std::string END_GUN_GIVER = "end gun giver";
const std::string BEGIN_RED_FLAG = "begin red flag";
const std::string END_RED_FLAG = "end red flag";
const std::string BEGIN_BLUE_FLAG = "begin blue flag";
const std::string END_BLUE_FLAG = "end blue flag";
const std::string BEGIN_RED_SPAWN = "begin red spawn";
const std::string END_RED_SPAWN = "end red spawn";
const std::string BEGIN_BLUE_SPAWN = "begin blue spawn";
const std::string END_BLUE_SPAWN = "end blue spawn";
const std::string BEGIN_OVERGROUND_IMAGE = "begin overground image";
const std::string END_OVERGROUND_IMAGE = "end overground image";
const std::string BEGIN_UNDERGROUND_IMAGE = "begin underground image";
const std::string END_UNDERGROUND_IMAGE = "end underground image";

bool saveOvergroundLevel(const string& levelName, shared_ptr<Floor>& floor, Flag& redFlag, Flag& blueFlag,
               const sf::FloatRect& redSpawn, const sf::FloatRect& blueSpawn) {

    //open the level file
    fstream file;

    //each level is saved into two portions, the overground and the underground portion
    //must add the word overground/underground to level name in order to differentiate the overground parts from the underground
    string overgroundTag = "overground";

    file.open(addFileExtention(levelName + overgroundTag), std::ios_base::out | std::ios_base::trunc);

    if(!file.good()) {

        cout << "Failed to open level: " << levelName << endl;
        return false;
    }

    //level is loaded now save all data
    if(!saveFloor(file, floor, BEGIN_OVERGROUND_IMAGE, END_OVERGROUND_IMAGE)) {

        return false;
    }

    if(!saveFlag(file, redFlag, BEGIN_RED_FLAG, END_RED_FLAG)) {

        cout << "failed to save red team flag" << endl;
        return false;
    }

    if(!saveFlag(file, blueFlag, BEGIN_BLUE_FLAG, END_BLUE_FLAG)) {

        cout << "failed to save blue team flag" << endl;
        return false;
    }

    if(!saveSpawn(file, redSpawn, BEGIN_RED_SPAWN, END_RED_SPAWN)) {

        cout << "failed to save red team spawn zone" << endl;
        return false;
    }

    if(!saveSpawn(file, blueSpawn, BEGIN_BLUE_SPAWN, END_BLUE_SPAWN)) {

        cout << "failed to save blue team spawn zone" << endl;
        return false;
    }

    file.close();

    return true;
}

bool saveUndergroundLevel(const string& levelName, shared_ptr<Floor>& floor) {

    //open the level file
    fstream file;

    //each level is saved into two portions, the overground and the underground portion
    //must add the word overground/underground to level name in order to differentiate the overground parts from the underground
    string undergroundTag = "underground";

    file.open(addFileExtention(levelName + undergroundTag), std::ios_base::out | std::ios_base::trunc);

    if(!file.good()) {

        cout << "Failed to open level: " << levelName << endl;
        return false;
    }

    //level is loaded now save all data
    if(!saveFloor(file, floor, BEGIN_UNDERGROUND_IMAGE, END_UNDERGROUND_IMAGE)) {

        return false;
    }

    file.close();

    return true;
}

bool loadOvergroundLevel(const string& levelName, shared_ptr<Floor>& floor, Flag& redFlag, Flag& blueFlag,
               sf::FloatRect& redSpawn, sf::FloatRect& blueSpawn) {

    //open the level file
    fstream file;

    //each level is saved into two portions, the overground and the underground portion
    //must add the word overground/underground to level name in order to differentiate the overground parts from the underground
    string overgroundTag = "overground";

    file.open(addFileExtention(levelName + overgroundTag), std::ios_base::in);

    if(!file.good()) {

        cout << "Failed to open level: " << levelName << endl;
        return false;
    }

    //level is opened so load data
    if(!loadFloor(file, floor, BEGIN_OVERGROUND_IMAGE, END_OVERGROUND_IMAGE)) {

        return false;
    }

    if(!loadFlag(file, redFlag, BEGIN_RED_FLAG, END_RED_FLAG)) {

        cout << "failed to load red team flag" << endl;
        return false;
    }

    if(!loadFlag(file, blueFlag, BEGIN_BLUE_FLAG, END_BLUE_FLAG)) {

        cout << "failed to load blue team flag" << endl;
        return false;
    }

    if(!loadSpawn(file, redSpawn, BEGIN_RED_SPAWN, END_RED_SPAWN)) {

        cout << "failed to load red team spawn zone" << endl;
        return false;
    }

    if(!loadSpawn(file, blueSpawn, BEGIN_BLUE_SPAWN, END_BLUE_SPAWN)) {

        cout << "failed to load blue team spawn zone" << endl;
        return false;
    }

    file.close();

    return true;
}

bool loadUndergroundLevel(const string& levelName, shared_ptr<Floor>& floor) {

    //open the level file
    fstream file;

    //each level is saved into two portions, the overground and the underground portion
    //must add the word overground/underground to level name in order to differentiate the overground parts from the underground
    string undergroundTag = "underground";

    file.open(addFileExtention(levelName + undergroundTag), std::ios_base::in);

    if(!file.good()) {

        cout << "Failed to open level: " << levelName << endl;
        return false;
    }

    //load data
    if(!loadFloor(file, floor, BEGIN_UNDERGROUND_IMAGE, END_UNDERGROUND_IMAGE)) {

        return false;
    }

    file.close();

    return true;
}

bool setFilePosition(const std::string& lineToSet, std::fstream& file) {

    //set the position of the stream operator ot the beginning of hte file
    //read data from the file until you read the given line, the stream operator is placed past the given line
    file.seekg(file.beg);

    std::string downloadedData("");

    while(getline(file, downloadedData)) {

        if(downloadedData == lineToSet) {

            //position is set to just past the line
            return true;
        }
    }

    //failed to find position
    return false;
}

const string addFileExtention(const string& fileName, const string& extention) {

    return fileName + extention;
}

bool saveFloor(fstream& file, std::tr1::shared_ptr<Floor>& floor, const string& backgroundBegin, const string& backgroundEnd) {

    if(!saveObjects(floor->blocks, BEGIN_BLOCKS, END_BLOCKS, file, &saveBlock)) {

        cout << "failed to save Blocks" << endl;
        return false;
    }

    if(!saveObjects(floor->foregroundObjects, BEGIN_FOREGROUND, END_FOREGROUND, file, &saveForeground)) {

        cout << "failed to save foreground" << endl;
        return false;
    }

    if(!saveObjects(floor->portals, BEGIN_PORTAL, END_PORTAL, file, &savePortal)) {

        cout << "failed to save portals" << endl;
        return false;
    }

    if(!saveObjects(floor->gunGivers, BEGIN_GUN_GIVER, END_GUN_GIVER, file, &saveGunGiver)) {

        cout << "failed to save Gun Givers" << endl;
        return false;
    }

    if(!saveBackground(file, floor->backgroundImage, backgroundBegin, backgroundEnd)) {

        cout << "Failed to save background: " << floor->backgroundImage.getImagePath() << endl;
        return false;
    }

    return true;
}

bool saveFlag(fstream& file, Flag& flag, const string& beginTag, const string& endTag) {

    file << beginTag << endl;

    file << flag.getSpawn().x << endl;
    file << flag.getSpawn().y << endl;

    file << endTag << endl;

    return file.good();
}

bool saveSpawn(fstream& file, const sf::FloatRect& spawnArea, const string& beginTag, const string& endTag) {

    file << beginTag << endl;

    file << spawnArea.left << endl;
    file << spawnArea.top << endl;
    file << spawnArea.width << endl;
    file << spawnArea.height << endl;

    file << endTag << endl;

    return file.good();
}

bool saveBackground(fstream& file, const LargeImage& image, const string& beginTag, const string& endTag) {

    file << beginTag << endl;

    file << image.getImagePath() << endl;

    file << endTag << endl;

    return file.good();
}

bool loadFloor(fstream& file, std::tr1::shared_ptr<Floor>& floor, const string& backgroundBegin, const string& backgroundEnd) {

    if(!loadObjects(floor->blocks, BEGIN_BLOCKS, END_BLOCKS, file, &loadBlock)) {

        cout << "failed to load Blocks" << endl;
        return false;
    }

    if(!loadObjects(floor->foregroundObjects, BEGIN_FOREGROUND, END_FOREGROUND, file, &loadForeground)) {

        cout << "failed to load foreground" << endl;
        return false;
    }

    if(!loadObjects(floor->portals, BEGIN_PORTAL, END_PORTAL, file, &loadPortal)) {

        cout << "failed to load portals" << endl;
        return false;
    }

    if(!loadObjects(floor->gunGivers, BEGIN_GUN_GIVER, END_GUN_GIVER, file, &loadGunGiver)) {

        cout << "failed to load Gun Givers" << endl;
        return false;
    }

    if(!loadBackground(file, floor->backgroundImage, backgroundBegin, backgroundEnd)) {

        cout << "Failed to load background" << endl;
        return false;
    }

    return true;
}

bool loadFlag(fstream& file, Flag& flag, const string& beginTag, const string& endTag) {

    //find the given tag
    if(!setFilePosition(beginTag, file)) {

        return false;
    }

    sf::Vector2f flagPosition;
    std::string loadedData;

    getline(file, loadedData);
    flagPosition.x = atof(loadedData.c_str());

    getline(file, loadedData);
    flagPosition.y = atof(loadedData.c_str());

    flag.setSpawn(flagPosition);

    return file.good();
}

bool loadSpawn(fstream& file, sf::FloatRect& spawnArea, const string& beginTag, const string& endTag) {

    //find the given tag
    if(!setFilePosition(beginTag, file)) {

        return false;
    }

    string loadedData;

    getline(file, loadedData);
    spawnArea.left = atof(loadedData.c_str());

    getline(file, loadedData);
    spawnArea.top = atof(loadedData.c_str());

    getline(file, loadedData);
    spawnArea.width = atof(loadedData.c_str());

    getline(file, loadedData);
    spawnArea.height = atof(loadedData.c_str());


    return file.good();
}

bool loadBackground(fstream& file, LargeImage& image, const string& beginTag, const string& endTag) {

    //find the given tag
    if(!setFilePosition(beginTag, file)) {

        return false;
    }

    string imagePath;

    //read the image path
    getline(file, imagePath);

    image.load(imagePath);

    return file.good();
}

void saveBlock(fstream& file, const shared_ptr<Block>& block) {

    sf::FloatRect blockDimensions = block->getCollisionBox();

    file << blockDimensions.left << endl;
    file << blockDimensions.top << endl;
    file << blockDimensions.width << endl;
    file << blockDimensions.height << endl;

    file << block->getPassBullets() << endl;
}

void saveForeground(std::fstream& file, const shared_ptr<ForegroundObject>& foregroundObj) {

    //save the name and location of the object
    file << foregroundObj->getImageName() << endl;
    file << foregroundObj->getPosition().x << endl;
    file << foregroundObj->getPosition().y << endl;
}

void savePortal(std::fstream& file, const shared_ptr<Portal>& portal) {

    //save the properties of the entrance and the properties and floor of the destination
    sf::FloatRect entrance = portal->getCollisionBox();

    file << entrance.left << endl;
    file << entrance.top << endl;
    file << entrance.width << endl;
    file << entrance.height << endl;

    sf::Vector2f exit = portal->getTeleportPosition();

    file << exit.x << endl;
    file << exit.y << endl;

    file << portal->getDestinationFloor() << endl;
}

void saveGunGiver(fstream& file, const shared_ptr<GunGiver>& gunGiver) {

    sf::Vector2f position = gunGiver->getPosition();

    file << position.x << endl;
    file << position.y << endl;

    file << gunGiver->getGunType() << endl;
}

shared_ptr<Block> loadBlock(fstream& file, const string& endTag) {

    std::string loadedData;

    //if the first line loaded from the file is the end tag it means there is no data to load for this object
    //or if the getline failed
    if(!getline(file, loadedData) || loadedData == endTag) {

        //empty pointer to indicate failure to load
        return shared_ptr<Block>();
    }

    sf::FloatRect bounds;

    //begin reading properties of the block
    bounds.left = atof(loadedData.c_str());

    getline(file, loadedData);
    bounds.top = atof(loadedData.c_str());

    getline(file, loadedData);
    bounds.width = atof(loadedData.c_str());

    getline(file, loadedData);
    bounds.height = atof(loadedData.c_str());

    bool passThrough = false;

    getline(file, loadedData);
    passThrough = atoi(loadedData.c_str());

    shared_ptr<Block> block(new Block(sf::Vector2f(bounds.width, bounds.height), passThrough));
    block->setPosition(sf::Vector2f(bounds.left, bounds.top));

    return block;
}

shared_ptr<ForegroundObject> loadForeground(fstream& file, const string& endTag) {

    string loadedData;

    //if the first line loaded from the file is the end tag it means there is no data to load for this object
    //or if the getline failed
    if(!getline(file, loadedData) || loadedData == endTag) {

        //empty pointer to indicate failure to load
        return shared_ptr<ForegroundObject>();
    }

    shared_ptr<ForegroundObject> foreground(new ForegroundObject(loadedData));

    sf::Vector2f position;

    getline(file, loadedData);

    position.x = atof(loadedData.c_str());
    getline(file, loadedData);

    position.y = atof(loadedData.c_str());
    foreground->setPosition(position);

    return foreground;
}
shared_ptr<Portal> loadPortal(fstream& file, const string& endTag) {

    string loadedData;

    //if the first line loaded from the file is the end tag it means there is no data to load for this object
    //or if the getline failed
    if(!getline(file, loadedData) || loadedData == endTag) {

        //empty pointer to indicate failure to load
        return shared_ptr<Portal>();
    }

    sf::FloatRect entrance;

    entrance.left = atof(loadedData.c_str());

    getline(file, loadedData);
    entrance.top = atof(loadedData.c_str());

    getline(file, loadedData);
    entrance.width = atof(loadedData.c_str());

    getline(file, loadedData);
    entrance.height = atof(loadedData.c_str());

    shared_ptr<Portal> portal(new Portal(sf::Vector2f(entrance.left, entrance.top), sf::Vector2f(entrance.width, entrance.height)));

    sf::Vector2f exit;

    getline(file, loadedData);
    exit.x = atof(loadedData.c_str());

    getline(file, loadedData);
    exit.y = atof(loadedData.c_str());

    unsigned destinationFloor = 0;

    getline(file, loadedData);
    destinationFloor = atoi(loadedData.c_str());

    portal->setTeleportPosition(exit);
    portal->setDestinationFloor(destinationFloor);

    return portal;
}

shared_ptr<GunGiver>loadGunGiver(fstream& file, const string& endTag) {

    string loadedData;

    //if the first line loaded from the file is the end tag it means there is no data to load for this object
    //or if the getline failed
    if(!getline(file, loadedData) || loadedData == endTag) {

        //empty pointer to indicate failure to load
        return shared_ptr<GunGiver>();
    }

    sf::Vector2f position;

    position.x = atof(loadedData.c_str());

    getline(file, loadedData);
    position.y = atof(loadedData.c_str());

    int gunType = 0;

    getline(file, loadedData);
    gunType = atoi(loadedData.c_str());

    shared_ptr<GunGiver> gunGiver(new GunGiver(static_cast<GunTypes>(gunType)));

    gunGiver->setPosition(position);

    return gunGiver;
}
