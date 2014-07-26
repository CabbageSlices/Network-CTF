#ifndef PACKETMANIPULATORS_H_INCLUDED
#define PACKETMANIPULATORS_H_INCLUDED

#include "SFML/Network.hpp"
#include "ServerGameManager.h"

#include <vector>
#include <tr1/memory>

//forward declaration
class UserPlayer;
class InterpolatingPlayer;
class FlagManager;

//all of these functions are helper functions used to make packet creation and reading easier for the users

//package up the player's latest, unsent inputs in order to send to server, place data into given packet
//return true if there was any data added to indicate whether or not the packet should be sent
//assumes packet is empty
//also adds the player's current rotation so server can send player's rotation to other clients
bool createInputPacket(const UserPlayer& player, sf::Packet& dataDestination);

//create a packet that contains the players current keystate
void createStatePacket(const UserPlayer& player, sf::Packet& dataDestination);

//create update packet to give to player
void createUpdatePacket(std::tr1::shared_ptr<FlagManager> flagManager, const UserPlayer& player, const sf::Uint32& lastConfirmedInput, sf::Packet& dataDestination);

//read the given position and state update from the server and apply the updates to the player
///this function assumes the outstream of the ip address has already read the packet id thus it will begin reading from the id of lasat confirmed input sent by player
/**
    CONTENTS OF UPDATE PACKET
    -PACKET ID
    -ID OF LAST CONFIRMED INPUT SENT BY PLAYER
    -PLAYER POSITION.x
    -PLAYER POSITION.y
    -PLAYER HEALTH
    -PLAYER'S TEAM
    -PLAYER HAS FLAG

    -------INFO ABOUT FLAGS
    -IF FLAG FOR TEAM 1 IS AT BASE
    -IF FLAG FOR TEAM 2 IS AT BASE
    -IF FLAG FOR TEAM 1 IS BEING HELD
    -IF FLAG FOR TEAM 2 IS BEING HELD
**/
void applyPlayerUpdate(std::tr1::shared_ptr<FlagManager> flagManager, UserPlayer& player, sf::Packet& updatePacket);

//state update packet contains data about all players connected to server
/**
    Formatting for state update packet
    -PACKET ID
    -PACKET TIME STAMP (to check if an old packet was sent)
    -NUMBER OF PLAYERS WHOSE DATA IS BEING SENT

    ----REPEATE THE FOLLOWING FOR EVERY PLAYER WHOSE DATA IS BEING SENT
    -ID OF PLAYER
    -PLAYER POSITION (x,y)
    -PLAYER ROTATION
    -PLAYER HEALTH
    -PLAYER TEAM
    -PLAYER HAS FLAG

    -NUMBER OF BULLETS FIRED
        ---REPEAT THE FOLLOWING FOR EVERY BULLET
        -BULLET BEGIN POINT
        -BULLET END POINT

**/
///its up to the receiving client to determine which player's data should not be used
void createStateUpdate(const std::vector<std::tr1::shared_ptr<ServerGameManager::ConnectedPlayer> >& players, const sf::Uint32& stateId, sf::Packet& statePacket);

//assumes packet id has already beenread from the state packet, ignores any packet that is older thant he stateid given
void applyStateUpdate(std::tr1::shared_ptr<FlagManager> flagManager, std::vector<std::tr1::shared_ptr<InterpolatingPlayer> >& players, UserPlayer& userPlayer, sf::Uint32& stateId, sf::Packet& statePacket);

//player firing data
/**

    -PACKET ID
    -NUMBER OF GUNSHOTS
    -FRACTION OF TIME PASSED SINCE LAST SERVER UPDATE (so server knows how far back in time to move all objects when cheking for collision with gunshot)
    -ID OF LAST UPDATE SENT BY SERVER (so server knows which update to start interpolating the world from before checkign for collision)

    **repeat the following for every gunshot player has sent
    -PLAYER ROTATION
**/
void createGunfirePacket(UserPlayer& player, const float& deltaFraction, const sf::Uint32& lastServerUpdate, sf::Packet& packet);

//find the player who shot and make him shoot,
///assumes id has NOT been read
void readGunfirePacket(UserPlayer& player, float& deltaFraction, sf::Uint32& lastServerUpdate, sf::Packet& packet);

//reads the packet and returns the packet type, moves the stream operator of hte packet
int getPacketType(sf::Packet& packet);

//reads the packet id of the packet and checks if the packet is the same type as the type sent to check
//after this function is used the packet stream operator is not moved because it takes a copy of the packet
///uses get packet type to check the packet type
bool checkPacketType(sf::Packet packet, const int& typeToCompare);

//empty packet used for latency testing with the server
sf::Packet createLatencyPacket();

#endif // PACKETMANIPULATORS_H_INCLUDED
