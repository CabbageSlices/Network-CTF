#include "LevelManager.h"
#include "Block.h"

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

bool saveLevel(const string& levelName, const vector<shared_ptr<Block> >& blocks) {

    //open the level file
    fstream file;

    file.open(addFileExtention(levelName), std::ios_base::out | std::ios_base::trunc);

    if(!file.good()) {

        cout << "Failed to open level: " << levelName << endl;
        return false;
    }

    //level is loaded now save all data
    if(!saveBlocks(blocks, file)) {

        cout << "failed to save Blocks" << endl;
        return false;
    }

    file.close();

    return true;
}

bool saveBlocks(const std::vector<std::tr1::shared_ptr<Block> >& blocks, std::fstream& file) {

    //put the blocks tag
    file << "blocks" << endl;

    //now save all of the blocks
    for(auto block : blocks) {

        //only save the position of the block because the size of the block is set by the class itself
        sf::Vector2f position = block->getPosition();

        //save x position, then a line, then a y position
        file << position.x << endl;
        file << position.y << endl;
    }

    //indicate the end of the blocks
    file << "end blocks" << endl;

    //check if the file is in an error state now
    if(!file.good()) {

        return false;
    }

    return true;
}

bool loadLevel(const string& levelName, vector<shared_ptr<Block> >& blocks) {

    //open the level file
    fstream file;

    file.open(addFileExtention(levelName), std::ios_base::in);

    if(!file.good()) {

        cout << "Failed to open level: " << levelName << endl;
        return false;
    }

    if(!loadBlocks(blocks, file)) {

        cout << "Failed to load Blocks" << endl;
        return false;
    }

    return true;
}

bool loadBlocks(vector<shared_ptr<Block> >& blocks, fstream& file) {

    //start reading from the beginning of the blocks by using the block tag
    if(!setFilePosition(BEGIN_BLOCKS, file)) {

        //failed to set position so blocksp robably not saved so exit
        return false;
    }

    //read lines until the end blocks tag is read, if its read then there are no more blocks to read or you reach the end of file
    std::string downloadedData;

    while(getline(file, downloadedData) && downloadedData != END_BLOCKS) {

        //read the x and y position and create a block at the given position
        sf::Vector2f position(0, 0);

        //x position
        getline(file, downloadedData);
        position.x = atoi(downloadedData.c_str());

        //y position
        getline(file, downloadedData);
        position.y = atoi(downloadedData.c_str());

        //create the block
        shared_ptr<Block> block(new Block(position));

        blocks.push_back(block);
    }

    //check if the stream is still in a valid state, if it isn't then reading failed
    if(!file.good()) {

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
