#include "LevelManager.h"
#include "Block.h"
#include "ForegroundObject.h"
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

const std::string BEGIN_BLOCKS = "start blocks";
const std::string END_BLOCKS = "end blocks";
const std::string BEGIN_FOREGROUND = "start foreground";
const std::string END_FOREGROUND = "end foreground";

bool saveLevel(const string& levelName, const vector<shared_ptr<Block> >& blocks, const vector<shared_ptr<ForegroundObject> >& foreground) {

    //open the level file
    fstream file;

    file.open(addFileExtention(levelName), std::ios_base::out | std::ios_base::trunc);

    if(!file.good()) {

        cout << "Failed to open level: " << levelName << endl;
        return false;
    }

    //level is loaded now save all data
    if(!saveObjects(blocks, BEGIN_BLOCKS, END_BLOCKS, file)) {

        cout << "failed to save Blocks" << endl;
        return false;
    }

    if(!saveObjects(foreground, BEGIN_FOREGROUND, END_FOREGROUND, file)) {

        cout << "failed to save foreground" << endl;
        return false;
    }

    file.close();

    return true;
}

bool loadLevel(const string& levelName, vector<shared_ptr<Block> >& blocks, vector<shared_ptr<ForegroundObject> >& foreground) {

    //open the level file
    fstream file;

    file.open(addFileExtention(levelName), std::ios_base::in);

    if(!file.good()) {

        cout << "Failed to open level: " << levelName << endl;
        return false;
    }

    if(!loadObjects(BEGIN_BLOCKS, END_BLOCKS, blocks, file)) {

        cout << "Failed to load Blocks" << endl;
        return false;
    }

    if(!loadObjects(BEGIN_FOREGROUND, END_FOREGROUND, foreground, file)) {

        cout << "Failed to load Foreground" << endl;
        return false;
    }

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

void saveObject(shared_ptr<Block> block, std::fstream& file) {

    sf::Vector2f position = block->getPosition();

    //save position
    file << position.x << std::endl;
    file << position.y << std::endl;
}
