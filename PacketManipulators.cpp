#include "PacketManipulators.h"
#include "PacketIdentification.h"
#include "UserPlayer.h"
#include "InterpolatingPlayer.h"

#include <iostream>
#include "Bullet.h"
#include "LineSegment.h"

using std::cout;
using std::endl;
using std::vector;
using std::tr1::shared_ptr;

bool createInputPacket(const UserPlayer& player, sf::Packet& dataDestination) {

    //set the id of the packet
    dataDestination << PLAYER_INPUT;

    //package up all of the players input and place them one at a time into a new packet
    const vector<UserPlayer::Input> playerInputs = player.getInputsToSend();

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

void createStatePacket(const UserPlayer& player, sf::Packet& dataDestination) {

    UserPlayer::KeyState keystate = player.getKeystate();

    dataDestination << PLAYER_KEYSTATE_UPDATE;

    dataDestination << keystate.pressedLeft;
    dataDestination << keystate.pressedRight;
    dataDestination << keystate.pressedUp;
    dataDestination << keystate.pressedDown;

    dataDestination << player.getRotation();

    dataDestination << keystate.inputId;
}

void createUpdatePacket(const UserPlayer& player, const sf::Uint32& lastConfirmedInput, sf::Packet& dataDestination) {

    //set the packet type
    dataDestination << PLAYER_STATE_UPDATE;

    dataDestination << lastConfirmedInput;

    sf::Vector2f playerPosition = player.getDestinationPosition();

    dataDestination << playerPosition.x;
    dataDestination << playerPosition.y;

    dataDestination << player.getHealth();
}

void applyPlayerUpdate(UserPlayer& player, sf::Packet& updatePacket) {

    sf::Uint32 inputId = 0;

    //read the id of the last input confirmed by the server
    updatePacket >> inputId;

    sf::Vector2f playerPosition;

    updatePacket >> playerPosition.x >> playerPosition.y;

    //create a player state so the player can read the data
    UserPlayer::State updatedState;
    updatedState.position = playerPosition;

    //read the players health on the server side
    int health = 0;
    updatePacket >> health;

    updatedState.health = health;

    //apply to player
    player.handleServerUpdate(updatedState, inputId);
}

void createStateUpdate(const vector<shared_ptr<ServerGameManager::ConnectedPlayer> >& players, const sf::Uint32& stateId, sf::Packet& updatePacket) {

    updatePacket << WORLD_STATE_UPDATE;

    updatePacket << stateId;

    //number of players whose data is saved onto the packet
    updatePacket << players.size();

    //add all the player's data to the packet
    for(auto player : players) {

        updatePacket << player->player.getId();
        updatePacket << player->player.getDestinationPosition().x << player->player.getDestinationPosition().y;
        updatePacket << player->player.getRotation();
        updatePacket << player->player.getHealth();

        vector<shared_ptr<Bullet> > bullets = player->player.getBullets();

        //indicate how many bullets were fired
        updatePacket << bullets.size();

        //add teh beginning and end locaitons of all bullets so the clients receiving the packet can create a bullet for this player so player can see where the shot was fired
        for(auto bullet : bullets) {

            sf::Vector2f beginPoint = bullet->getLine()->getStartPoint();
            sf::Vector2f endPoint = bullet->getLine()->getEndPoint();

            updatePacket << beginPoint.x << beginPoint.y;
            updatePacket << endPoint.x << endPoint.y;
        }
    }
}

void createGunfirePacket(UserPlayer& player, const float& deltaFraction, const sf::Uint32& lastServerUpdate, sf::Packet& packet) {

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

void readGunfirePacket(UserPlayer& player, float& deltaFraction, sf::Uint32& lastServerUpdate, sf::Packet& packet) {

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

void applyStateUpdate(vector<shared_ptr<InterpolatingPlayer> >& players, UserPlayer& userPlayer, sf::Uint32& stateId, sf::Packet& statePacket) {

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

        int playerHealth = 0;
        statePacket >> playerHealth;

        //read the number of bullets fired by the player, use floatrects to hold the data for the bullet's begin and end points
        //top left is the begin point, width and heights are the end point
        vector<sf::FloatRect> bullets;

        int bulletsFired = 0;
        statePacket >> bulletsFired;

        for(int bulletsCreated = 0; bulletsCreated < bulletsFired; bulletsCreated++) {

            sf::FloatRect position;
            statePacket >> position.left >> position.top >> position.width >> position.height;

            bullets.push_back(position);
        }

        //don't update him if the given player is also the user player
        //because the user player is updated seperately from the other players because the user player actually takes input while the others only interpolate
        ///however the data from above must be read because the data needs to be extracted from the packet otherwise you'll read the wrong data
        if(playerId == userPlayer.getId()) {

            //just move onto the next one
            continue;
        }

        //if the given player already exists then update him, however
        bool playerAlreadyExists = false;

        //player who is being updated
        shared_ptr<InterpolatingPlayer> updatedPlayer;

        for(auto player : players) {

            if(player->getId() == playerId) {

                updatedPlayer = player;
                playerAlreadyExists = true;
                break;
            }
        }

        if(!playerAlreadyExists) {

           //player does not exist so create him
            shared_ptr<InterpolatingPlayer> newPlayer(new InterpolatingPlayer());
            updatedPlayer = newPlayer;

            //save the data of the new player
            players.push_back(newPlayer);
        }


        updatedPlayer->setId(playerId);
        updatedPlayer->setInterpolationPosition(playerPosition);
        updatedPlayer->setInterpolationRotation(playerRotation);
        updatedPlayer->setHealth(playerHealth);

        //now create the bullets for this player
        for(auto bullet : bullets) {

            sf::Vector2f bulletStart(bullet.left, bullet.top);
            sf::Vector2f bulletEnd(bullet.width, bullet.height);

            updatedPlayer->fireGun(bulletStart, bulletEnd);
        }
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
