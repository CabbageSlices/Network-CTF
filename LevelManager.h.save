#ifndef LEVELMANAGER_H_INCLUDED
#define LEVELMANAGER_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

#include <string>
#include <vector>
#include <tr1/memory>
#include <fstream>

class Block;

///tags to indcate what objects are being saved or loaded so you can easily search through files
extern const std::string BEGIN_BLOCKS;
extern const std::string END_BLOCKS;


//functions to load and save levels
//returns true if operation is successful and false if faile
///assumes the levelname is just the name of hte level without the file extention
bool saveLevel(const std::string& levelName, const std::vector<std::tr1::shared_ptr<Block> >& blocks);

//save the data of teh blocks using the given file
bool saveBlocks(const std::vector<std::tr1::shared_ptr<Block> >& blocks, std::fstream& file);

bool loadLevel(const std::string& levelName, std::vector<std::tr1::shared_ptr<Block> >& blocks);

bool loadBlocks(std::vector<std::tr1::shared_ptr<Block> >& blocks, std::fstream& file);

//sets the stream position of the file directly past the lineToSet, meaning the lineToSet is ignored
//returns true if the line is found and position is set, false otherwise
//the given file must be able to perfom input operations
bool setFilePosition(const std::string& lineToSet, std::fstream& inputFile);

//returns the given string with the file extention added
const std::string addFileExtention(const std::string& fileName, const std::string& extention = ".txt");

#endif // LEVELMANAGER_H_INCLUDED
