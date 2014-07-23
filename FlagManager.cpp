#include "FlagManager.h"
#include "TeamManager.h"

#include <stdexcept>
#include <iostream>

using std::map;
using std::out_of_range;
using std::cout;
using std::endl;
using std::tr1::shared_ptr;

FlagManager::FlagManager(const sf::Vector2f& teamASpawn, const sf::Vector2f& teamBSpawn):
    flags()
    {
        addFlag(teamASpawn, TEAM_A_ID);
        addFlag(teamBSpawn, TEAM_B_ID);
    }

void FlagManager::setFlagPosition(const sf::Vector2f& position, unsigned short teamId) {

    //if the flag with the given team id does not exist then indicate to the player an error occured and close the game
    try {

        flags.at(teamId)->flag.setPosition(position);

    } catch(out_of_range& error) {

        cout << "A team's id is invalid and does not match one of the flags" << endl;
    }
}

void FlagManager::draw(sf::RenderWindow& window) {

    for(auto& flag : flags) {

        window.draw(flag.second->flag);
    }
}

void FlagManager::addFlag(const sf::Vector2f& flagSpawnLocation, unsigned short teamId) {

    //create a new flag at the given position
    shared_ptr<Flag> flag(new Flag());
    flag->flag.setPosition(flagSpawnLocation);

    flags[teamId] = flag;
}
