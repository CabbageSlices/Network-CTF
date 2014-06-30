#include "PacketManipulators.h"
#include "PacketIdentification.h"
#include "player.h"

#include <iostream>
#include "Bullet.h"

using std::cout;
using std::endl;
using std::vector;
using std::tr1::shared_ptr;

bool createInputPacket(const Player& player, sf::Packet& dataDestination) {

    //set the id of the packet
    dataDestination << PLAYER_INPUT;

    //package up all of the players input and place them one at a time into a new packet
    const vector<Player::Input> playerInputs = player.getInputsToSend();

    //first thing the input packet should contain is the number of inputs so the server knows how many inputs to read
    dataDestination << playerInputs.size();

    for(unsigned int index = 0; index < playerInputs.size(); index++) {

        //place player inputs into packet, convert the enum 'action' to an int first
        dataDestination << static_cast<int>(playerInputs[index].action);
        dataDestination << playerInputs[index].inputId;
    }

    dataDestination << player.getRotation();

    //if the number of inputs sent is 0 then there was no data added, so there is only data added if the number of player inputs is more than 0
    return playerInputs.size() > 0;
}

void createUpdatePacket(const Player& player, const sf::Uint32& lastConfirmedInput, sf::Packet& dataDestination) {

    //set the packet type
    dataDestination << PLAYER_STATE_UPDATE;

    dataDestination << lastConfirmedInput;

    sf::Vector2f playerPosition = player.getPosition();

    dataDestination << playerPosition.x;
    dataDestination << playerPosition.y;
}

void applyPlayerUpdate(Player& player, sf::Packet& updatePacket) {

    sf::Uint32 inputId = 0;

    //read the id of the last input confirmed by the server
    updatePacket >> inputId;

    sf::Vector2f playerPosition;

    updatePacket >> playerPosition.x >> playerPosition.y;

    //create a player state so the player can read the data
    Player::State updatedState;
    updatedState.position = playerPosition;

    //apply to player
    player.handleServerUpdate(updatedState, inputId);
}

void createStateUpdate(const vector<shared_ptr<ServerGameManager::ConnectedPlayer> >& players, const sf::Uint32& stateId, sf::Packet& updatePacket) {

    updatePacket << WORLD_STATE_UPDATE;

    updatePacket << stateId;

    //number of players whose data is saved onto the packet
    updatePacket << players.size();

    //add all the player's data to the packet, except for the player receiving the data because he doesn't need to know his own state
    for(auto player : players) {

        updatePacket << player->player.getId();
        updatePacket << player->player.getPosition().x << player->player.getPosition().y;
        updatePacket << player->player.getRotation();

        vector<shared_ptr<Bullet> > bullets = player->player.getBullets();

        updatePacket << bullets.size();
    }
}

void createGunfirePacket(Player& player, const float& deltaFraction, const sf::Uint32& lastServerUpdate, sf::Packet& packet) {

    //id the packet
    packet << PLAYER_GUNFIRE;

    const vector<float> playerGunshotRotations = player.getGunshotsToSend();

    packet << playerGunshotRotations.size();

    packet << deltaFraction;

    packet << lastServerUpdate;

    for(unsigned gunshotsCreated = 0; gunshotsCreated < playerGunshotRotations.size(); gunshotsCreated++) {

        packet << playerGunshotRotations[gunshotsCreated];
    }
}

void readGunfirePacket(Player& player, float& deltaFraction, sf::Uint32& lastServerUpdate, sf::Packet& packet) {

    ///first read packet id since it hasn't been extracted yet
    int packetId = 0;

    packet >> packetId;

    //read the number of gunshots fired
    unsigned gunshots = 0;

    packet >> gunshots;

    packet >> deltaFraction;

    packet >> lastServerUpdate;

    //create gunshots for this player until there are no more gunshots left to created
    for(unsigned createdShots = 0; createdShots < gunshots; createdShots++) {

        float rotation = 0;

        packet >> rotation;

        player.fireGun(rotation);

        player.setRotation(rotation);
    }
}

void applyStateUpdate(vector<shared_ptr<Player> >& players, Player& userPlayer, sf::Uint32& stateId, sf::Packet& statePacket) {

    sf::Uint32 packetStateId = 0;

    statePacket >> packetStateId;

    if(packetStateId < stateId) {

        ///old data, ignore
        return;
    }

    stateId = packetStateId;

    //determine how many updates the pakcet has
    unsigned int numberOfPlayers = 0;

    statePacket >> numberOfPlayers;

    //receive data about all players packet has information about
    for(unsigned int counter = 0; counter < numberOfPlayers; counter++) {

        int playerId = 0;
        statePacket >> playerId;

        sf::Vector2f playerPosition(0, 0);
        statePacket >> playerPosition.x >> playerPosition.y;

        float playerRotation;
        statePacket >> playerRotation;

        //don't update him if the given player is also the user player
        //because the user player is updated seperately from the other players because the user player actually takes input while the others only interpolate
        if(playerId == userPlayer.getId()) {

            //just move onto the next one
            continue;
        }

        //if the given player already exists then update him, however
        bool playerAlreadyExists = false;

        for(auto player : players) {

            if(player->getId() == playerId) {

                player->setInterpolationPosition(playerPosition);
                player->setInterpolationRotation(playerRotation);
                playerAlreadyExists = true;
                break;
            }
        }

        if(playerAlreadyExists) {

            //this player has already been updated so just restart the loop and move onto the next one
            continue;
        }

        //player does not exist so create him
        shared_ptr<Player> newPlayer(new Player());
        newPlayer->setId(playerId);
        newPlayer->setInterpolationPosition(playerPosition);
        newPlayer->setInterpolationRotation(playerRotation);

        players.push_back(newPlayer);
    }
}

int getPacketType(sf::Packet& packet) {

    //extract the first peice of data which is the packet type
    int packetType;
    packet >> packetType;

    return packetType;
}

bool checkPacketType(sf::Packet packet, const int& typeToCompare) {

    return (getPacketType(packet) == typeToCompare);
}

sf::Packet createLatencyPacket() {

    sf::Packet latencyPacket;
    latencyPacket << LATENCY_TEST;

    return latencyPacket;
}
