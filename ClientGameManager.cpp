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
#include <map>
#include <iterator>

/**





MAKE CLIENT GAME MANAGER HAVE THE FUNCTION TO CALCULATE DELTA FRACTION FOR SERVER UPDATES AND THEN SEND THE DATA
TO THE SERVER
**/

using std::tr1::shared_ptr;
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::map;
using std::next;

ClientGameManager::ClientGameManager() :
    GameManager(),
    currentState(STATE_PLAYING),
    client("70.71.114.74", 8080),
    userPlayer(),
    camera(),
    connectedPlayers(),
    stateUpdateTimer(),
    serverUpdateTime(sf::milliseconds(60)),
    lastStateUpdateId(0),
    waitingForOthers(true)
    {

    }

void ClientGameManager::setPlayerName(string name) {

    userPlayer.setName(name);
}

bool ClientGameManager::connectToServer(string serverIp, unsigned short serverPort) {

    int playerId = 0;

    client.setServerIp(serverIp);
    client.setServerPort(serverPort);

    if(client.connectToServer(playerId, userPlayer.getName(), sf::seconds(2))) {

        userPlayer.setId(playerId);

        return true;
    }

    return false;
}

void ClientGameManager::gameLobby(sf::RenderWindow& window, sf::Font& font) {

    //load the image of the game lobby
    sf::Texture lobbyTexture;
    lobbyTexture.loadFromFile("lobbyMenu.png");

    sf::Sprite lobbySprite;
    lobbySprite.setTexture(lobbyTexture);

    //names of each player in their respective teams to draw onto the screen
    //has no use what so ever outside the lobby
    vector<sf::Text> redTeam;
    vector<sf::Text> blueTeam;

    //size of box for each player name in the lobby image
    sf::Vector2f nameBoxSize(279, 52);

    //position on the lobby image where first name is displayed
    sf::Vector2f firstNamePosition(49, 84);

    //the different buttons users can press
    sf::FloatRect backButton(512, 4, 125, 61);
    sf::FloatRect switchTeamsButton(249, 334, 147, 65);

    sf::Event event;

    //set window to default properties that way all objects are drawn on screen
    window.setView(window.getDefaultView());

    sf::Clock dataReceiveTimer;
    sf::Time timeOutTime = sf::seconds(3);

    //timer to send heartbeat packets to server
    sf::Clock heartbeatTimer;
    sf::Time heartBeatTime = sf::seconds(1);

    while(window.isOpen()) {

        while(window.pollEvent(event)) {

            if(event.type == sf::Event::Closed) {

                window.close();
            }

            if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {

                sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if(backButton.contains(mousePosition)) {
                    return;
                }

                if(switchTeamsButton.contains(mousePosition)) {

                    sf::Packet packet;

                    //tell server user wants to switch teams
                    packet << CHANGE_TEAM;

                    client.sendToServer(packet);
                }
            }
        }

        if(heartbeatTimer.getElapsedTime() > heartBeatTime) {

            sf::Packet packet;
            packet << HEART_BEAT;

            client.sendToServer(packet);

            heartbeatTimer.restart();
        }

        //recreate the list of players in each time whenever the server sends an update
        sf::Packet packet;

        if(dataReceiveTimer.getElapsedTime() > timeOutTime) {

            ///do something to draw that you've disconnected
            return;
        }

        //get new data from the server
        while(client.receiveFromServer(packet)) {

            dataReceiveTimer.restart();

            int packetType = getPacketType(packet);

            if(packetType == LOBBY_CONNECTION_INFO) {

                redTeam.clear();
                blueTeam.clear();

                //first read the number of points required to win
                packet >> pointsToWinGame;

                while(!packet.endOfPacket()) {

                    //read data about all players, first thing read is the player name
                    string playerName;

                    unsigned short team = 0;

                    packet >> playerName;
                    packet >> team;

                    sf::Text nameText;
                    nameText.setFont(font);
                    nameText.setString(playerName);
                    nameText.setScale(0.70, 0.70);

                    if(team == TEAM_A_ID) {

                        redTeam.push_back(nameText);

                    } else {

                        blueTeam.push_back(nameText);
                    }
                }

            } else if(packetType == START_GAME) {

                //run the game, will go to the game stage but won't start until server sends state update packet
                runGame(window);

                //reset the view that way you cna see the menu
                window.setView(window.getDefaultView());

                //reset the data receive timer because the time will be very high right now
                //and clients will get disconnected
                dataReceiveTimer.restart();
            }
        }

        //set the position of all names so that they line up with the boxes in the image file
        for(unsigned i = 0; i < redTeam.size(); i++) {

            sf::Text& name = redTeam[i];
            name.setPosition(firstNamePosition.x, firstNamePosition.y + i * nameBoxSize.y);
        }

        for(unsigned int i = 0; i < blueTeam.size(); i++) {

            sf::Text& name = blueTeam[i];
            name.setPosition(firstNamePosition.x + nameBoxSize.x, firstNamePosition.y + i * nameBoxSize.y);
        }

        window.clear();

        window.draw(lobbySprite);

        for(auto& name : redTeam) {

            window.draw(name);
        }

        for(auto& name : blueTeam) {

            window.draw(name);
        }

        window.display();
    }
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
    float deltaFraction = stateUpdateTimer.getElapsedTime().asSeconds() / serverUpdateTime.asSeconds();

    return deltaFraction;
}


void ClientGameManager::sendInputsToServer() {

    sf::Packet packagedInputs;

    //if game is over jst tell the server teh player is currently in the end game screen
    if(matchEnded) {

        packagedInputs << AT_END_SCREEN;
        client.sendToServer(packagedInputs);
        return;
    }

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

    //don't send updates if game is over
    if(matchEnded) {

        return;
    }

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

            if(applyStateUpdate(getFlagManager(), connectedPlayers, userPlayer, lastStateUpdateId, downloadedData)) {

                serverUpdateTime = stateUpdateTimer.restart();

                //if player is receiving state updates that meanst he game already started
                waitingForOthers = false;
            }

            //when looking at victory or defeat only load textures
            //if they haven't been loaded already
            //textures haven't been loaded if matchEnded is still false
        } else if(packetType == VICTORY) {

            if(matchEnded == false) {

                loadVictoryTexture(matchResultTexture);
                matchResultSprite.setTexture(matchResultTexture);
            }

            matchEnded = true;

            //after match ends the client begins to ignore server packets that give score updates
            //so the team scores aren't updated
            //add the last score to whatever team won
            ///increases player's team score
            if(userPlayer.getTeam() == TEAM_A_ID) {

                getHeadsUpDisplay().getScoreDisplay().setRedScore(pointsToWinGame);

            } else {

                getHeadsUpDisplay().getScoreDisplay().setBlueScore(pointsToWinGame);
            }

        } else if(packetType == DEFEAT) {

            if(matchEnded == false) {

                loadDefeatTexture(matchResultTexture);
                matchResultSprite.setTexture(matchResultTexture);
            }

            matchEnded = true;

            //after match ends the client begins to ignore server packets that give score updates
            //so the team scores aren't updated
            //add the last score to whatever team won
            ///increases other teams score
            if(userPlayer.getTeam() == TEAM_A_ID) {

                getHeadsUpDisplay().getScoreDisplay().setBlueScore(pointsToWinGame);

            } else {

                getHeadsUpDisplay().getScoreDisplay().setRedScore(pointsToWinGame);
            }

        } else if(packetType == LOBBY_CONNECTION_INFO) {

            //servers at the lobby so client should return as well
            ///exitGameLoop = true;
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

    //setup camera
    camera.setDefaultSize(window);

    //setup the player name on the H.U.D
    getStatDisplay().setPlayerName(userPlayer.getName());
    getStatDisplay().setPlayerHealth(userPlayer.getHealth());

    //set player's health to 0 that way player can't move until all other players are loaded
    //and also so player will set his position to server's position when the game starts
    userPlayer.setHealth(0);

    waitingForOthers = true;
    matchEnded = false;

    //delete all previous players so new ones can be created
    connectedPlayers.clear();

    //clear scoreboards so they draw the correct scores the next time players are created
    score.clearScoreboard();
}

void ClientGameManager::handleWindowEvents(sf::Event& event, sf::RenderWindow& window) {

    //window size was changed so change the camera
    if(event.type == sf::Event::Resized) {

        camera.setDefaultSize(window);
    }
}

void ClientGameManager::handleComponentInputs(sf::Event& event, sf::RenderWindow& window) {

    score.handleEvents(event);

    //don't handle player input once game is over
    if(matchEnded) {

        return;
    }

    //handle the player's inputs
    userPlayer.handleEvents(event);

}

void ClientGameManager::updateComponents(sf::RenderWindow& window) {

    sendInputsToServer();
    sendGunshotsToServer();
    handleServerUpdates();

    updateScoreboard(window);

    updateStatDisplay();

    //don't handle bullet collision once match is over
    if(matchEnded) {

        return;
    }

    ///handle the bullet collision here isntead of in the time components
    ///update part because the loop may never run because there isn't enough
    ///time accumulate for an update loop to occur, if that happens you get
    ///weird bullet drawins because the bullet never checked for collision
    handleBulletCollision();
}

void ClientGameManager::updateTimeComponents(const float& delta, sf::RenderWindow& window) {

    //don't update anything if match is over
    if(matchEnded) {

        return;
    }

    //only update the player if he is alive
    if(userPlayer.isAlive()) {

        updateUserPlayer(delta, window);
    }

    updateConnectedPlayers(delta);
}

void ClientGameManager::handlePostUpdate(sf::RenderWindow& window) {

    //don't update anything if match is over
    if(matchEnded) {

        return;
    }

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

    //if player is waiting for others don't draw the screen
    userPlayer.draw(window);

    for(auto& player : connectedPlayers) {

        if(player->getFloor() == userPlayer.getFloor()) {

            player->draw(window, userPlayer.getFloor());
        }
    }

    if(waitingForOthers) {

        drawWaitingSymbol(window);
    }

    //if the match is already over then draw the appropriate texture
    if(matchEnded) {

        window.draw(matchResultSprite);
    }
}

void ClientGameManager::drawUI(sf::RenderWindow& window) {

    if(score.canDisplayInfo()) {

        score.draw(window);
    }

    getHeadsUpDisplay().drawGunUI(window, userPlayer.getGun());
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
