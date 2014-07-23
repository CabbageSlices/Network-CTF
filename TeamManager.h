#ifndef TEAMMANAGER_H_INCLUDED
#define TEAMMANAGER_H_INCLUDED

#include <map>

//class to keep track of how many players are in each team and assign a team to a new player
class TeamManager {

    private:

        //counter for the number of players in each team
        //map key is the team id, and the value is the number of players for that team
        std::map<unsigned short, int> numberOfPlayers;

        void setupPlayerCounter();

    public:

        TeamManager();

        //add a new player and choose what team to add him to
        //returns the team that the player was added to
        unsigned short addNewPlayer();

        //remove a player from the given team if possible
        void removePlayer(const unsigned short& teamId);

        //take a player from the given team and move it to the other team
        //return id of the team that player switched to
        unsigned short switchTeams(unsigned short currentTeamId);
};

extern const unsigned short TEAM_A_ID;
extern const unsigned short TEAM_B_ID;

#endif // TEAMMANAGER_H_INCLUDED
