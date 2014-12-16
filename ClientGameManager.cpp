#include "ClientGameManager.h"
#include "PacketManipulators.h"
#include "PacketIdentification.h"
#include "Bullet.h"
#include "LineSegment.h"
#include "Collision.h"
#include "InterpolatingPlayer.h"
#include "ForegroundObject.h"
#include "TeamManager.h"
#include "Block.h"
#include "Gun.h"

#include <cstdlib>
#include <iostream>

/**





MAKE CLIENT GAME MANAGER HAVE THE FUNCTION TO CALCULATE DELTA FRACTION FOR SERVER UPDATES AND THEN SEND THE DATA
TO THE SERVER
**/

using std::tr1::shared_ptr;
using std::cout;
using std::endl;
using std::vector;

ClientGameManager::ClientGameManager() :
    GameManager(),
    currentState(STATE_PLAYING),
    client("70.71.114.74", 8080),
    userPlayer(),
    camera(),
    connectedPlayers(),
    stateUpdateTimer(),
    serverUpdateTime(sf::milliseconds(60)),
    lastStateUpdateId(0)
    {

    }

void ClientGameManager::interpolateEntities() {

    ///delta fraction for user player and other players are different
    ///because userplayer is interpolated based on physics update rate
    ///and other players are updated at a different time interval (time interval is dependant on server, for now its 60 milliseconds)
    userPlayer.interpolate(calculateDeltaFraction());

    getGameWorld().updateFlagPosition(userPlayer);

    //make sure the denominator isn't 0
    //calculate the delta fraction to interpolate other connected players
    //this uses the time between server updates for interpolation rather than time between physics update cycle
    float deltaFraction = calculateServerDelta();

    for(auto& player : connectedPlayers) {

        player->interpolate(deltaFraction);

        getGameWorld().updateFlagPosition(*player);
    }
}

const float ClientGameManager::calculateServerDelta() {

    //make sure the denominator isn't 0
    //calculate the delta fraction to interpolate other connected players
    //this uses the time between server updates for interpolation rather than time between physics update cycle
    float deltaFraction = stateUpdateTimer.getElapsedTime().asSeconds() / (serverUpdateTime.asSeconds() > 0.f ? serverUpdateTime.asSeconds() : 1.f);

    return deltaFraction;
}


void ClientGameManager::sendInputsToServer() {

    sf::Packet packagedInputs;

    //if there was any new inputs added, then send to server
    if(createInputPacket(userPlayer, packagedInputs)) {

        client.sendToServer(packagedInputs);
    }

    userPlayer.clearInputsToSend();

    //now send the players state if needed
    if(userPlayer.shouldSendKeystate()) {

        sf::Packet statePacket;
        createStatePacket(userPlayer, statePacket);
        userPlayer.resetKeystateTimer();

        client.sendToServer(statePacket);
    }
}

void ClientGameManager::sendGunshotsToServer() {

    //check if the user shot any bullets and send any gunshot data to the server
    if(userPlayer.getGunshotsToSend().size() == 0) {

        return;
    }

    sf::Packet packetToSend;

    const float deltaFraction = calculateServerDelta();

    createGunfirePacket(userPlayer, deltaFraction, lastStateUpdateId, packetToSend);

    //clear the queued gunfires
    userPlayer.clearGunshotQueue();

    client.sendToServer(packetToSend);
}

void ClientGameManager::handleServerUpdates() {

    //try and receive data and if there is any data read it
    sf::Packet downloadedData;

    //not server data so no point handling the data
    while(client.receiveFromServer(downloadedData)) {

        //check the type of data received
        const int packetType = getPacketType(downloadedData);

        //respond to the packet depending on the type of packet
        if(packetType == PLAYER_STATE_UPDATE) {

            applyPlayerUpdate(getFlagManager(), userPlayer, downloadedData, getScoreDisplay());


        } else if(packetType == WORLD_STATE_UPDATE) {

            applyStateUpdate(getFlagManager(), connectedPlayers, userPlayer, lastStateUpdateId, downloadedData);
            serverUpdateTime = stateUpdateTimer.restart();

            cout << serverUpdateTime.asMilliseconds() - 60 << endl;
        }

    }
}

void ClientGameManager::updateScoreboard(sf::RenderWindow& window) {

    if(score.canDisplayInfo()) {

        score.setDisplayCenter(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
        score.clearScoreboard();
        score.addPlayerInfo(userPlayer);
        score.updatePlayerList(connectedPlayers);
        score.setupInfoPosition();
    }
}

void ClientGameManager::updateUserPlayer(const float& delta, sf::RenderWindow& window) {

    userPlayer.update(delta, sf::Vector2f(window.getSize().x, window.getSize().y));
    userPlayer.updateRotation(window.mapPixelToCoords(sf::Mouse::getPosition(window) ));
}

void ClientGameManager::updateConnectedPlayers(const float& delta) {

    for(unsigned int index = 0; index < connectedPlayers.size();) {

        if(connectedPlayers[index]->timedOut()) {

            connectedPlayers.erase(connectedPlayers.begin() + index);
            continue;
        }

        connectedPlayers[index]->updateGun(delta);
        ++index;
    }
}

void ClientGameManager::updateStatDisplay() {

    getStatDisplay().setPlayerHealth(userPlayer.getHealth());

    getStatDisplay().setPlayerAmmo(userPlayer.getGun()->getCurrentAmmo(),
                                   userPlayer.getGun()->getMaxCurrentAmmo(),
                                   userPlayer.getGun()->getTotalAmmo());
}

void ClientGameManager::handleBulletCollision() {

    //get the bullets from the player and check for collision with others
    vector<shared_ptr<Bullet> > bullets = userPlayer.getBullets();

    for(auto& bullet : bullets) {

        if(!bullet->checkCanCollide()) {

            continue;
        }

        //first check for colision with blocks, bullets only collide with blocks that don't let bullets pass
        bulletEntityCollision<Block>(bullet, getGameWorld().getBlocks(bullet->getLine()->getStartPoint(), bullet->getLine()->getEndPoint(), userPlayer.getFloor()),
                                     [&](shared_ptr<Block> block) {

                                     return !block->getPassBullets();
                                     });

        //handle collision with other players
        bulletEntityCollision<InterpolatingPlayer>(bullet, connectedPlayers,
                                                   [&](shared_ptr<InterpolatingPlayer> player)->bool {

                                                    return !(player->getTeam() == userPlayer.getTeam()) && player->isAlive() && player->getFloor() == bullet->getFloor();
                                                   });

        //disable the bullets collisoin since it should no longerb e able to collide
        bullet->disableCollision();
    }
}

void ClientGameManager::playerForegroundCollision() {

    //if the player or his teammates are hidinging behind something make it visible so you can see
    for(auto& entity : getForeground(userPlayer.getFloor())) {

        bool hidingPlayer = false;

        //check if the player is hiding behind the cover
        if(userPlayer.getDestinationBox().intersects(entity->getCollisionBox())) {

            hidingPlayer = true;

        }

        //check if any of your teammates are hiding behind the foreground
        for(auto player : connectedPlayers) {

            if(player->getDestinationBox().intersects(entity->getCollisionBox()) && player->getTeam() == userPlayer.getTeam()) {

                hidingPlayer = true;
            }
        }

        entity->setHidingPlayer(hidingPlayer);
    }
}

void ClientGameManager::setup(sf::RenderWindow& window) {

    int playerId = 0;

    if(client.connectToServer(playerId)) {

        userPlayer.setId(playerId);

    } else {

        cout << "Failed to receive data from server.";
        ///window.close();
    }

    //setup camera
    camera.setDefaultSize(window);

    //setup the player name on the H.U.D
    getStatDisplay().setPlayerName(userPlayer.getName());
    getStatDisplay().setPlayerHealth(userPlayer.getHealth());
}

void ClientGameManager::handleWindowEvents(sf::Event& event, sf::RenderWindow& window) {

    //window size was changed so change the camera
    if(event.type == sf::Event::Resized) {

        camera.setDefaultSize(window);
    }
}

void ClientGameManager::handleComponentInputs(sf::Event& event, sf::RenderWindow& window) {

    //handle the player's inputs
    userPlayer.handleEvents(event);

    score.handleEvents(event);
}

void ClientGameManager::updateComponents(sf::RenderWindow& window) {

    sendInputsToServer();
    sendGunshotsToServer();
    handleServerUpdates();

    updateScoreboard(window);

    updateStatDisplay();

    ///handle the bullet collision here isntead of in the time components
    ///update part because the loop may never run because there isn't enough
    ///time accumulate for an update loop to occur, if that happens you get
    ///weird bullet drawins because the bullet never checked for collision
    handleBulletCollision();
}

void ClientGameManager::updateTimeComponents(const float& delta, sf::RenderWindow& window) {

    //only update the player if he is alive
    if(userPlayer.isAlive()) {

        updateUserPlayer(delta, window);
    }

    updateConnectedPlayers(delta);
}

void ClientGameManager::handlePostUpdate(sf::RenderWindow& window) {

    //linearly interpolate all entities to their destination positions
    interpolateEntities();

    //apply camera before setting the center that way camera is already resized before it is set in the center
    //because applycamera causes camera to zoom in or out if the camera started zooming
    camera.applyCamera(window);

    //move camera to new track players new position, unless he died then leave it as is
    ///currently there is no level bounds or properties so just give some default level properties for now
    if(userPlayer.isAlive()) {

        camera.setCameraCenter(sf::Vector2f(userPlayer.getCollisionBox().left, userPlayer.getCollisionBox().top));
    }
}

void ClientGameManager::drawComponents(sf::RenderWindow& window) {

    userPlayer.draw(window);

    for(auto& player : connectedPlayers) {

        if(player->getFloor() == userPlayer.getFloor()) {

            player->draw(window, userPlayer.getFloor());
        }
    }
}

void ClientGameManager::drawUI(sf::RenderWindow& window) {

    if(score.canDisplayInfo()) {

        score.draw(window);
    }
}

void ClientGameManager::drawMinimap(sf::RenderWindow& window) {

    //only draw on minimap if they are on the overground floor
    if(userPlayer.getFloor() == OVERGROUND_FLOOR) {

        userPlayer.drawMinimap(window);
    }


    ///only draw players on minimap if they are in the overground section
    //only draw another player on the minimap if they are on the players team, or they are near someone on the players team, and only if they are overground
    for(auto& player : connectedPlayers) {

        if((player->getTeam() == userPlayer.getTeam() || (player->getCollisionBox().intersects(camera.getCameraBounds()) && userPlayer.getFloor() == OVERGROUND_FLOOR)) && player->getFloor() == OVERGROUND_FLOOR) {

            player->drawMinimap(window);

        } else {

            //not on player's team, so figure out if he is near a teammate
            for(auto teammate : connectedPlayers) {

                //if the player is within the screens of the user's team mate and they are on the overground floor then draw him on the minimap
                if(teammate->getTeam() == userPlayer.getTeam() && player->getCollisionBox().intersects(camera.getCameraBounds(teammate->getCurrentPosition())) &&
                   player->getFloor() == OVERGROUND_FLOOR && teammate->getFloor() == OVERGROUND_FLOOR) {

                    player->drawMinimap(window);

                    //already drawn so leave loop
                    break;
                }
            }
        }
    }
}

const unsigned ClientGameManager::getFloor() const {

    return userPlayer.getFloor();
}

void ClientGameManager::handleCollisions() {

    //no broadphase as of yet
    //check for collision between players and blocks
    //top left and right points of the player's collision box which will be used to determine which blocks he can collide with
    sf::Vector2f topLeft(userPlayer.getCollisionBox().left, userPlayer.getCollisionBox().top);
    sf::Vector2f topRight(userPlayer.getCollisionBox().left + userPlayer.getCollisionBox().width, topLeft.y);

    playerStaticCollision(userPlayer, getGameWorld().getBlocks(topLeft, topRight, userPlayer.getFloor()));

    playerForegroundCollision();
}
