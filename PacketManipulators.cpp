#include "PacketManipulators.h"
#include "PacketIdentification.h"
#include "UserPlayer.h"
#include "InterpolatingPlayer.h"
#include "TeamManager.h"
#include "PlayerBase.h"
#include "Flag.h"
#include "Gun.h"

#include <iostream>
#include "Bullet.h"
#include "LineSegment.h"

using std::cout;
using std::endl;
using std::vector;
using std::tr1::shared_ptr;

//gives or takes a flag from a player depending on update from the server
void updateFlagInfo(const bool& holdingFlagServer, const sf::Vector2f& serverPosition, PlayerBase& player, shared_ptr<FlagManager> flagManager) {

    //if player picked up a flag then give him a flag
    if(holdingFlagServer && !player.isHoldingFlag()) {

        player.holdFlag(flagManager->getFlag(getOpposingTeam(player.getTeam()) ));

    } else if(player.isHoldingFlag() && !holdingFlagServer) {

        //player was holding the flag and now he isn't, means he dropped the flag or he scored
        //drop the flag at the given server position
        //only way player can drop flag is if he is killed, so if is still alive it means he dropped the flag because he scored
        bool scored = player.isAlive();

        player.dropFlag(serverPosition);

        if(scored) {

            flagManager->resetFlags();
        }
    }
}

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

void createUpdatePacket(shared_ptr<FlagManager> flagManager, const UserPlayer& player, const sf::Uint32& lastConfirmedInput, sf::Packet& dataDestination) {

    //set the packet type
    dataDestination << PLAYER_STATE_UPDATE;

    dataDestination << lastConfirmedInput;

    sf::Vector2f playerPosition = player.getDestinationPosition();

    dataDestination << playerPosition.x;
    dataDestination << playerPosition.y;

    dataDestination << player.getHealth();
    dataDestination << player.getTeam();
    dataDestination << player.isHoldingFlag();

    dataDestination << player.getKills();
    dataDestination << player.getDeaths();
    dataDestination << player.getFlagCaptures();
    dataDestination << player.getFlagReturns();
    dataDestination << player.getFloor();
    dataDestination << player.getGun()->getCurrentAmmo();
    dataDestination << player.getGun()->getTotalAmmo();

    dataDestination << flagManager->teamAFlag()->isAtSpawn() << flagManager->teamBFlag()->isAtSpawn();
    dataDestination << flagManager->teamAFlag()->isHeld() << flagManager->teamBFlag()->isHeld();

    dataDestination << flagManager->teamAFlag()->getPosition().x << flagManager->teamAFlag()->getPosition().y;
    dataDestination << flagManager->teamBFlag()->getPosition().x << flagManager->teamBFlag()->getPosition().y;
}

void applyPlayerUpdate(shared_ptr<FlagManager> flagManager, UserPlayer& player, sf::Packet& updatePacket) {

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

    unsigned short team;
    updatePacket >> team;

    updatedState.team = team;

    //apply to player
    player.handleServerUpdate(updatedState, inputId);

    bool holdingFlag = false;
    updatePacket >> holdingFlag;

    updateFlagInfo(holdingFlag, playerPosition, player, flagManager);

    unsigned short kills = 0;
    unsigned short deaths = 0;
    unsigned short flagCaptures = 0;
    unsigned short flagReturns = 0;

    updatePacket >> kills >> deaths >> flagCaptures >> flagReturns;

    player.setKills(kills);
    player.setDeaths(deaths);
    player.setCaptures(flagCaptures);
    player.setReturns(flagReturns);

    unsigned floor = 0;
    updatePacket >> floor;

    player.setFloor(floor);

    int currentAmmo = 0;
    int totalAmmo = 0;

    updatePacket >> currentAmmo >> totalAmmo;

    player.getGun()->setCurrentAmmo(currentAmmo);
    player.getGun()->setTotalAmmo(totalAmmo);

    //whether any team flags should be returned to base or dropped
    bool flagABase = false;
    bool flagBBase = false;
    bool flagAHeld = false;
    bool flagBHeld = false;

    updatePacket >> flagABase >> flagBBase >> flagAHeld >> flagBHeld;

    if(flagABase) {

        flagManager->teamAFlag()->reset();
    }

    if(flagBBase) {

        flagManager->teamBFlag()->reset();
    }

    //if flags are not held hten make them drop, even if a player is holding a flag if the server says the flag is not being held
    //if means the player holding the flag disconnected so make the flag drop on client side, its fine to dorp at whatever position its currently at
    //because most likely the player who held the flag discconnected from the last position the client saw him at
    if(!flagAHeld) {

        flagManager->teamAFlag()->dropFlag();
    }

    if(!flagBHeld) {

        flagManager->teamBFlag()->dropFlag();
    }

    sf::Vector2f flagAPosition;
    sf::Vector2f flagBPosition;

    updatePacket >> flagAPosition.x >> flagAPosition.y;
    updatePacket >> flagBPosition.x >> flagBPosition.y;

    if(!flagABase && !flagAHeld) {

        flagManager->teamAFlag()->setPosition(flagAPosition);
    }

    if(!flagBBase && !flagBHeld) {

        flagManager->teamBFlag()->setPosition(flagBPosition);
    }
}

void createStateUpdate(const vector<shared_ptr<ServerGameManager::ConnectedPlayer> >& players, const sf::Uint32& stateId, sf::Packet& updatePacket) {

    updatePacket << WORLD_STATE_UPDATE;

    updatePacket << stateId;

    //number of players whose data is saved onto the packet
    updatePacket << players.size();

    //add all the player's data to the packet
    for(auto& player : players) {

        updatePacket << player->player.getId();
        updatePacket << player->player.getDestinationPosition().x << player->player.getDestinationPosition().y;
        updatePacket << player->player.getRotation();
        updatePacket << player->player.getHealth();
        updatePacket << player->player.getTeam();
        updatePacket << player->player.isHoldingFlag();
        updatePacket << player->player.getFloor();

        vector<shared_ptr<Bullet> > bullets = player->player.getBullets();

        //indicate how many bullets were fired
        updatePacket << bullets.size();

        //add teh beginning and end locaitons of all bullets so the clients receiving the packet can create a bullet for this player so player can see where the shot was fired
        for(auto& bullet : bullets) {

            sf::Vector2f beginPoint = bullet->getLine()->getStartPoint();
            sf::Vector2f endPoint = bullet->getLine()->getEndPoint();

            updatePacket << beginPoint.x << beginPoint.y;
            updatePacket << endPoint.x << endPoint.y;
            updatePacket << bullet->getFloor();
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

void applyStateUpdate(shared_ptr<FlagManager> flagManager, vector<shared_ptr<InterpolatingPlayer> >& players, UserPlayer& userPlayer, sf::Uint32& stateId, sf::Packet& statePacket) {

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

        unsigned short teamId = 0;
        statePacket >> teamId;

        bool holdingFlag = false;
        statePacket >> holdingFlag;

        unsigned floor = 0;
        statePacket >> floor;

        struct BulletData {

            sf::Vector2f beginPosition;
            sf::Vector2f endPosition;
            unsigned floor;
        };

        //read the number of bullets fired by the player
        vector<BulletData> bullets;

        int bulletsFired = 0;
        statePacket >> bulletsFired;

        for(int bulletsCreated = 0; bulletsCreated < bulletsFired; bulletsCreated++) {

            BulletData data;
            statePacket >> data.beginPosition.x >> data.beginPosition.y;
            statePacket >> data.endPosition.x >> data.endPosition.y;
            statePacket >> floor;

            bullets.push_back(data);
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

        //check if the updated player respawned, that is, he was dead previously and now his health is greater than 0
        bool respawned = !updatedPlayer->isAlive() && (playerHealth > 0);

        if(respawned) {

            updatedPlayer->respawn(playerPosition);
        }

        updatedPlayer->setId(playerId);
        updatedPlayer->setInterpolationPosition(playerPosition);
        updatedPlayer->setInterpolationRotation(playerRotation);
        updatedPlayer->setHealth(playerHealth);
        updatedPlayer->setTeam(teamId);
        updatedPlayer->setFloor(floor);

        updateFlagInfo(holdingFlag, playerPosition, *updatedPlayer, flagManager);

        //now create the bullets for this player
        for(auto& bullet : bullets) {

            updatedPlayer->fireGun(bullet.beginPosition, bullet.endPosition, bullet.floor);
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
