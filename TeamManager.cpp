#include "TeamManager.h"

const unsigned short TEAM_A_ID = 1;
const unsigned short TEAM_B_ID = 2;

const sf::Color TEAM_A_COLOR = sf::Color::Red;
const sf::Color TEAM_B_COLOR = sf::Color::Blue;

unsigned short getOpposingTeam(const unsigned short& currentTeam) {

    return (currentTeam == TEAM_A_ID) ? TEAM_B_ID : TEAM_A_ID;
}

const sf::Color getTeamColor(const unsigned short& team) {

    return (team == TEAM_A_ID) ? TEAM_A_COLOR : TEAM_B_COLOR;
}

TeamManager::TeamManager():
    numberOfPlayers()
    {
        setupPlayerCounter();
    }

unsigned short TeamManager::addNewPlayer() {

    //id of team that the player was added to
    int team = TEAM_B_ID;

    //add the player to whatever team has the fewer number of players
    if(numberOfPlayers[TEAM_A_ID] <= numberOfPlayers[TEAM_B_ID]) {

        numberOfPlayers[TEAM_A_ID]++;
        team = TEAM_A_ID;

    } else {

        numberOfPlayers[TEAM_B_ID]++;
        team = TEAM_B_ID;
    }

    return team;
}

void TeamManager::removePlayer(const unsigned short& team) {

    if(numberOfPlayers[team] > 0) {

        numberOfPlayers[team]--;
    }
}

short unsigned int TeamManager::switchTeams(unsigned short currentTeam) {

    int destinationTeam = (currentTeam == TEAM_A_ID) ? TEAM_B_ID : TEAM_A_ID;

    removePlayer(currentTeam);
    numberOfPlayers[destinationTeam]++;

    return destinationTeam;
}

void TeamManager::setupPlayerCounter() {

    numberOfPlayers[TEAM_A_ID] = 0;
    numberOfPlayers[TEAM_B_ID] = 0;
}
