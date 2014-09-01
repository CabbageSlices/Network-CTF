#include "ServerGameManager.h"
#include "PacketIdentification.h"
#include "PacketManipulators.h"
#include "Bullet.h"
#include "LineSegment.h"
#include "FlagManager.h"
#include "Collision.h"
#include "math.h"
#include "Block.h"
#include "Floors.h"
#include "Portal.h"
#include "GunGiver.h"

#include <map>
#include <iostream>

using std::map;
using std::cout;
using std::endl;
using std::tr1::shared_ptr;
using std::vector;

ServerGameManager::ServerGameManager(unsigned short portToBindTo) :
    GameManager(),
    server(false),
    players(),
    inputConfirmationTime(),
    inputConfirmationDelay(sf::milliseconds(40)),
    lastStateUpdateId(0),
    stateUpdateTimer(),
    stateUpdateDelay(sf::milliseconds(60)),
    lastPlayerId(0),
    teamManager()
    {
        server.bind(portToBindTo);
    }

void ServerGameManager::handleIncomingData() {

    //try and receive data and if there is any valid data then process it according to the data type
    sf::Packet incomingData;
    sf::IpAddress senderIp;
    unsigned short senderPort;

    while(server.receiveData(incomingData, senderIp, senderPort)) {

        //data is available, check if the sender already has a player connected
        for(auto& player : players) {

            if(player->playerIpAddress == senderIp && player->playerPort == senderPort) {

                handleData(player, incomingData);
                return;
            }
        }

        //data was not handled yet so check the data type and handle accordingly
        if(checkPacketType(incomingData, CONNECTION_ATTEMPT)) {

            //if connection attempt has not been handled yet it means the player has not been added yet, so add him and respond with the player's id
            createNewConnection(senderIp, senderPort, ++lastPlayerId);

            //tell the player who connected that he is connected by sending back the connection request packet
            //except this time add the player's id so the client knows what his own id is
            incomingData << lastPlayerId;
            server.sendData(incomingData, senderIp, senderPort);
            cout << "new connection" << endl;
        }

    }
}

void ServerGameManager::handleData(shared_ptr<ConnectedPlayer> player, sf::Packet& data) {

    //if the packet is a connection request then just tell the client he has already connected
    if(checkPacketType(data, CONNECTION_ATTEMPT)) {

        //add the player's id to the end of hte packet so client can figure out its id
        data << player->player.getId();
        server.sendData(data, player->playerIpAddress, player->playerPort);

        cout << "Connection Attempt" << endl;

    } else if(checkPacketType(data, PLAYER_INPUT)) {

        handlePlayerInput(player, data);
    } else if(checkPacketType(data, PLAYER_GUNFIRE)) {

        handlePlayerGunfire(player, data);
    } else if(checkPacketType(data, PLAYER_KEYSTATE_UPDATE)) {

        handlePlayerKeystate(player, data);
    }
}

void ServerGameManager::handlePlayerInput(shared_ptr<ConnectedPlayer> player, sf::Packet& inputPacket) {

    //first peice of data on the input packet is the packet type because it hasn't been removed yet so remove it
    int packetType = 0;
    inputPacket >> packetType;

    //next data is the number of inputs the player created
    int inputCount = 0;
    inputPacket >> inputCount;

    //handle inputs until there are no more inputs left to handle
    for(int inputsCreated = 0; inputsCreated < inputCount; inputsCreated++) {

        //read the input and make the player handle the given input
        //set initial input to -1 that way if it fails to read the input then player doesn't handle it
        int playerInput = -1;
        inputPacket >> playerInput;

        sf::Uint32 inputId = 0;
        inputPacket >> inputId;

        UserPlayer::Input clientInput;
        clientInput.inputId = inputId;
        clientInput.action = static_cast<UserPlayer::Action>(playerInput);

        //check if this is a newer input, if it isn't then ignore it
        if(inputId >= player->lastConfirmedInputId) {

            player->player.handleClientInput(clientInput);
            player->lastConfirmedInputId = inputId;
        }
    }

    //last data is the player's rotation
    float rotation = 0;
    inputPacket >> rotation;
    player->player.setRotation(rotation);

}

void ServerGameManager::handlePlayerKeystate(shared_ptr<ConnectedPlayer> player, sf::Packet& statePacket) {

    //packet type hasn't been read from packet so remove it
    int packetType = 0;
    statePacket >> packetType;

    //read the keystate now
    UserPlayer::KeyState keystate;

    statePacket >> keystate.pressedLeft;
    statePacket >> keystate.pressedRight;
    statePacket >> keystate.pressedUp;
    statePacket >> keystate.pressedDown;

    float rotation = 0;
    statePacket >> rotation;

    //read the id of this state update
    sf::Uint32 inputId = 0;
    statePacket >> inputId;

    //if the input id is older than the last input processed by the server it means its old data so ignore
    if(inputId < player->lastConfirmedInputId) {

        return;
    }

    player->lastConfirmedInputId = inputId;

    player->player.handleClientKeystate(keystate);
    player->player.setRotation(rotation);
}

void ServerGameManager::handlePlayerGunfire(shared_ptr<ConnectedPlayer> player, sf::Packet& inputPacket) {

    //what fraction of time has passed on the clients side since the server last sent an update
    float deltaFraction = 0;

    //id of last update the client received from the server in order to figure out where to start interpolating the world from in order
    //to check for collision with the gunfire, default value is the latest data
    sf::Uint32 clientUpdateId = lastStateUpdateId;

    readGunfirePacket(player->player, deltaFraction, clientUpdateId, inputPacket);

    handleGunfireCollision(player, deltaFraction, clientUpdateId);
}

void ServerGameManager::handleGunfireCollision(shared_ptr<ConnectedPlayer> player, const float& deltaFraction, const sf::Uint32& clientUpdateId) {

    //get all of the bullets from the player and check if any of them collide with any other objects
    vector<shared_ptr<Bullet> > bullets = player->player.getBullets();

    //check if each active bullet collides with anything and determine the first object it collides with
    for(auto& bullet : bullets) {

        if(!bullet->checkCanCollide()) {

            //can't collide so just move on to next bullet
            continue;
        }

        ///first check if the bullet collides with any obstacles because when it does it's range is shortened
        ///and bullets can't pass through blocks and such so once the range is shortened it will automatically prevent it from shooting players behind walls
        bulletEntityCollision<Block>(bullet, getGameWorld().getBlocks(bullet->getLine()->getStartPoint(), bullet->getLine()->getEndPoint(), player->player.getFloor()));

        //now handle collision with players
        handleBulletCollision(player, bullet, deltaFraction, clientUpdateId);

        bullet->disableCollision();
    }
}

void ServerGameManager::handleBulletCollision(shared_ptr<ConnectedPlayer> shootingPlayer, shared_ptr<Bullet> bullet, const float& deltaFraction, const sf::Uint32& clientUpdateId) {

    //move every player back in time to what the client was seeing when he fired the bullet, and check for collision between the bullet
    //and the player
    //determine the player that was first hit and handle collision with him

    //default nearest collision is the shooter
    shared_ptr<ConnectedPlayer> nearestPlayer = shootingPlayer;
    sf::Vector2f nearestCollisionPoint = bullet->getLine()->getEndPoint();

    for(auto& player : players) {

        if(player == shootingPlayer || player->player.getTeam() == shootingPlayer->player.getTeam() || !player->player.isAlive()) {

            continue;
        }

        //if player and bullet aren't on same floor don't check collision
        if(player->player.getFloor() != bullet->getFloor()) {

            continue;
        }

        //get the position of this player at the time the bullet was fired, default position is the player's destination position
        sf::Vector2f pastPosition(player->player.getDestinationPosition());

        //try to get the players position in the past, if a state tracker doesn't exist it means this player has no older positions so just use his current position
        //make sure there are states saved
        if(player->state && player->state->getStateCount() > 0) {

            //client update id is the current state update on the client side, it is the destination state that the entities on the client are interpolating towards
            //the starting point for the interpolation is the state id - 1
            pastPosition = player->state->approximatePosition(clientUpdateId - 1, deltaFraction);
        }

        //set the player to this position and check for collision between the bullet and the player
        sf::FloatRect collisionBox = player->player.getCurrentHitbox();

        //the collision box has to be centered  at the pastposition because the past position is the center of the player
        collisionBox.left = pastPosition.x - collisionBox.width / 2;
        collisionBox.top = pastPosition.y - collisionBox.height / 2;

        //point of collision if there is one
        sf::Vector2f collisionPoint(0, 0);

        //if there is a collision determine if the player that was shot is the nearest player
        if(checkCollision(collisionBox, bullet->getLine(), collisionPoint) &&
            distanceToPoint(bullet->getLine()->getStartPoint(), collisionPoint) < distanceToPoint(bullet->getLine()->getStartPoint(), nearestCollisionPoint)) {

            //this collision point is now the nearest point of collision so this player is the closest
            nearestPlayer = player;
            nearestCollisionPoint = collisionPoint;
        }
    }

    //now collide with the nearest player if the nearest player is not the shooter
    if(nearestPlayer != shootingPlayer) {

        //player was hit just make the line smaller and indicate it collided with something
        nearestPlayer->player.getHit(bullet->getDamage());
        bullet->setEndPoint(nearestCollisionPoint);
        bullet->disableCollision();

        //if the player being hit died then increase the kill count for the shooter
        //if the nearest player was hit it means he was alive before getting hit because of previous for loop
        if(!nearestPlayer->player.isAlive()) {

            shootingPlayer->player.setKills(shootingPlayer->player.getKills() + 1);
        }
    }
}

void ServerGameManager::createNewConnection(sf::IpAddress& connectedIp, unsigned short& connectedPort, const int& playerId) {

    shared_ptr<ConnectedPlayer> player(new ConnectedPlayer());
    player->player.setId(playerId);
    player->player.setTeam(teamManager.addNewPlayer());

    player->playerIpAddress = connectedIp;
    player->playerPort = connectedPort;
    player->lastConfirmedInputId = 0;

    players.push_back(player);
}

void ServerGameManager::sendInputConfirmation() {

    //for every client connected send them their position update along with the last input that the server confirmed from them
    for(auto& player : players) {

        sf::Packet playerUpdate;
        createUpdatePacket(getFlagManager(), player->player, player->lastConfirmedInputId, playerUpdate);

        //send the info to the player
        server.sendData(playerUpdate, player->playerIpAddress, player->playerPort);
    }
}

void ServerGameManager::sendStateUpdates() {

    sf::Packet stateUpdatePacket;

    //create a state update packet and send it to all clients connected
    createStateUpdate(players, lastStateUpdateId, stateUpdatePacket);

    //save the state before its sent
    savePlayerStates(lastStateUpdateId);
    lastStateUpdateId++;

    for(auto& player : players) {

        server.sendData(stateUpdatePacket, player->playerIpAddress, player->playerPort);
    }
}

void ServerGameManager::savePlayerStates(const int stateId) {

    //loop through all the players and update their state, if they don't exist then add a state for them
    for(auto& player : players) {

        //a record doesn't exist, so create a new one
        if(!player->state) {

            player->state.reset(new StateTracker(player->player.getId(), calculateMaxStatesSaved() ));
        }

        //update the state
        player->state->insertState(stateId, player->player.getDestinationPosition());
    }
}

int ServerGameManager::calculateMaxStatesSaved() {

    //number of states saved is equal to the number of updates sent to the clients in one second, rounded up (always)
    //calculations done in milliseconds
    return (1000 / stateUpdateDelay.asMilliseconds()) + 1;
}

void ServerGameManager::spawnPlayer(shared_ptr<ServerGameManager::ConnectedPlayer>& player) {

    player->player.respawn(getGameWorld().getSpawnPoint(player->player.getTeam()));

    player->player.setFloor(OVERGROUND_FLOOR);
}

void ServerGameManager::disconnectPlayer(unsigned playerIndex) {

    if(playerIndex > players.size()) {

        return;
    }

    //remove him from the current team so the teammanager can assign
    //his team to another player who connects
    teamManager.removePlayer(players[playerIndex]->player.getTeam());

    players.erase(players.begin() + playerIndex);
}

void ServerGameManager::drawPlayers(sf::RenderWindow& window) {

    for(auto& player : players) {

        player->player.draw(window);
    }
}

void ServerGameManager::playerSpawnCollision(shared_ptr<ServerGameManager::ConnectedPlayer>& player) {

    const sf::FloatRect& spawnArea = getGameWorld().getSpawnArea(player->player.getTeam());

    if(player->player.getCollisionBox().intersects(spawnArea) && player->player.isAlive()) {

        player->player.regenerateHealth();
    }
}

void ServerGameManager::setup(sf::RenderWindow& window) {

    ///no setup needed
    ///intentionally blank (for now)
}

void ServerGameManager::handleWindowEvents(sf::Event& event, sf::RenderWindow& window) {

    ///intentionally left blank
}

void ServerGameManager::handleComponentInputs(sf::Event& event, sf::RenderWindow& window) {

    ///no components to handle input from just yet
}

void ServerGameManager::updateComponents(sf::RenderWindow& window) {

    //receive new data
    handleIncomingData();

    //tell clients what the last input the server confirmed was
    if(inputConfirmationTime.getElapsedTime() > inputConfirmationDelay) {

        sendInputConfirmation();
        inputConfirmationTime.restart();
    }

    //send information about other players to connected clients
    //however if there are less than 2 players then theres no need to update them
    //because there are no other player's whose data must be sent
    if(stateUpdateTimer.getElapsedTime() > stateUpdateDelay && players.size() > 1) {

        sendStateUpdates();
        stateUpdateTimer.restart();
    }
}

void ServerGameManager::updateTimeComponents(const float& delta, sf::RenderWindow& window) {

    //update the players of all the connected clients, if any have timed out, delete them
    for(unsigned index = 0; index < players.size();) {

        if(players[index]->player.timedOut()) {

            disconnectPlayer(index);
            continue;
        }

        //if the player if alive update him, otherwise try to respawn him
        if(players[index]->player.isAlive()) {

            players[index]->player.update(delta, sf::Vector2f(window.getSize().x, window.getSize().y));

        } else if(players[index]->player.shouldRespawn()) {

            spawnPlayer(players[index]);
        }


        index++;
    }
}

void ServerGameManager::handlePostUpdate(sf::RenderWindow& window) {

    for(auto& player : players) {

        player->player.interpolate(calculateDeltaFraction());
        getGameWorld().updateFlagPosition(player->player);
    }
}

void ServerGameManager::drawComponents(sf::RenderWindow& window) {

    drawPlayers(window);
}

const unsigned ServerGameManager::getFloor() const {

    return OVERGROUND_FLOOR;
}

void ServerGameManager::handleCollisions() {

    //handle collision between player and all other objects that he can collide with
    //do in this one loop so you don't repeat loops
    for(auto& player : players) {

        //top left and right points of the player's collision box which will be used to determine which blocks he can collide with
        sf::Vector2f topLeft(player->player.getCollisionBox().left, player->player.getCollisionBox().top);
        sf::Vector2f topRight(player->player.getCollisionBox().left + player->player.getCollisionBox().width, topLeft.y);

        playerStaticCollision(player->player, getGameWorld().getBlocks(topLeft, topRight, player->player.getFloor()));
        playerStaticCollision(player->player, getPortals(player->player.getFloor()));
        playerStaticCollision(player->player, getGunGivers(player->player.getFloor()));

        collidePlayerFlag(player->player, *getFlagManager(), teamManager);

        playerSpawnCollision(player);
    }
}
