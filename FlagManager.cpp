#include "FlagManager.h"
#include "TeamManager.h"
#include "math.h"
#include "Flag.h"

#include <stdexcept>
#include <iostream>

#include "PlayerBase.h"

using std::map;
using std::out_of_range;
using std::cout;
using std::endl;
using std::tr1::shared_ptr;

void collidePlayerFlag(PlayerBase& player, FlagManager& flagManager, TeamManager& teamManager) {

    bool collidesOwnFlag = flagManager.checkFlagCollision(player.getCollisionBox(), player.getTeam());

    if(collidesOwnFlag) {

        ownFlagCollision(player, flagManager, teamManager);
    }

    bool collidesOpponentFlag = flagManager.checkFlagCollision(player.getCollisionBox(), getOpposingTeam(player.getTeam()));

    if(collidesOpponentFlag) {

        opponentFlagCollision(player, flagManager, teamManager);
    }
}

void ownFlagCollision(PlayerBase& player, FlagManager& flagManager, TeamManager& teamManager) {

    //flag can be returned to his base if no one is holding it and its not already at base
    bool canReturnFlag = flagManager.canHoldFlag(player.getTeam()) && !flagManager.flagAtSpawn(player.getTeam());

    //check for collision between each flag with each player and handle collision accordingly
    if(canReturnFlag) {

        //return flag to base
        flagManager.flagToSpawn(player.getTeam());

    } else if(flagManager.flagAtSpawn(player.getTeam()) && player.isHoldingFlag()) {

        //player is bringing the opponent's flag to base and his team's flag is already at his base
        //so this player scores scores, reset all flags
        player.dropFlag();
        flagManager.resetFlags();
        teamManager.increaseTeamScore(player.getTeam());
    }
}

void opponentFlagCollision(PlayerBase& player, FlagManager& flagManager, TeamManager& teamManager) {

    //id of the team the current player is not on, so the team that is opposing hte players current team
    unsigned short idOpposingTeam = getOpposingTeam(player.getTeam());

    if(flagManager.canHoldFlag(idOpposingTeam) && player.isAlive()) {

        //make player start holding the given flag
        player.holdFlag(flagManager.getFlag(idOpposingTeam));
    }
}

FlagManager::FlagManager(const sf::Vector2f& teamASpawn, const sf::Vector2f& teamBSpawn):
    flags()
    {
        addFlag(teamASpawn, TEAM_A_ID);
        addFlag(teamBSpawn, TEAM_B_ID);
    }

void FlagManager::setFlagPosition(const sf::Vector2f& position, unsigned short teamId) {

    //if the flag with the given team id does not exist then indicate to the player an error occured and close the game
    try {

        flags.at(teamId)->setPosition(position);

    } catch(out_of_range& error) {

        cout << "A team's id is invalid and does not match one of the flags" << endl;
    }
}

bool FlagManager::canHoldFlag(unsigned short teamId) {

    //flag can be taken if its not being held already
    return !flags[teamId]->isHeld();
}

bool FlagManager::flagAtSpawn(unsigned short teamId) {

    return flags[teamId]->isAtSpawn();
}

void FlagManager::holdFlag(unsigned short teamId) {

    flags[teamId]->pickUpFlag();
}

void FlagManager::dropFlag(unsigned short teamId) {

    flags[teamId]->dropFlag();
}

void FlagManager::resetFlags() {

    flagToSpawn(TEAM_A_ID);
    flagToSpawn(TEAM_B_ID);
}

bool FlagManager::checkFlagCollision(const sf::FloatRect& collisionRect, const unsigned short& teamId) {

    return collisionRect.intersects(flags[teamId]->getCollisionBox());
}

void FlagManager::flagToSpawn(const unsigned short& teamId) {

    flags[teamId]->reset();
}

shared_ptr<Flag> FlagManager::getFlag(unsigned short teamId) {

    return flags[teamId];
}

shared_ptr<Flag> FlagManager::teamAFlag() {

    return getFlag(TEAM_A_ID);
}

shared_ptr<Flag> FlagManager::teamBFlag() {

    return getFlag(TEAM_B_ID);
}

void FlagManager::draw(sf::RenderWindow& window) {

    for(auto& flag : flags) {

        flag.second->draw(window);
    }
}

void FlagManager::addFlag(const sf::Vector2f& flagSpawnLocation, unsigned short teamId) {

    //create a new flag at the given position
    shared_ptr<Flag> flag(new Flag(flagSpawnLocation,  getTeamColor(teamId) ));

    flags[teamId] = flag;
}
