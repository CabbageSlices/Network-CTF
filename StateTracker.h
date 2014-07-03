#ifndef STATETRACKER_H_INCLUDED
#define STATETRACKER_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

#include <vector>

//server uses to keep track of a player's state up to 1 second in the past
class StateTracker {

    private:

        //tie a position update to a time stamp so its easy to locate a position
        struct stateUpdate{

            sf::Uint32 stateId;
            sf::Vector2f position;
        };

        int playerId;

        std::vector<stateUpdate> pastStates;

        //max number of updates that should exist in the containers
        const unsigned int maxStatesSaved;

    public:

        StateTracker(const int& playerId, const int& updatesSaved);

        void insertState(const sf::Uint32& stateId, const sf::Vector2f& position);

        //approximate the state of the player given the last state and fraction of time passed between given state and next state
        //finds an approximate position of the player that way you can check for collision or other stuff to see if clients interacted with player
        //this function returns a position that exists between the given state and the next state
        ///do not use the state a player is interpolating towards because that is the wrong data
        ///if the player is currently at state n, then you must use this function and start interpolating from state n - 1
        sf::Vector2f approximatePosition(const sf::Uint32& idInitalState, const float& deltaFraction);

        //get the number of states currently saved in the tracker
        unsigned getStateCount();

        const int getPlayerId();

};

#endif // POSITIONTRACKER_H_INCLUDED
