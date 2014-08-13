#ifndef SERVERGAMEMANAGER_H_INCLUDED
#define SERVERGAMEMANAGER_H_INCLUDED

#include "SFML/Network.hpp"
#include "UserPlayer.h"
#include "ConnectionManager.h"
#include "GameManager.h"
#include "StateTracker.h"
#include "TeamManager.h"
#include <vector>
#include <tr1/memory>

class Bullet;

class ServerGameManager : public GameManager {

    public:///define stuff here so both public and private functions can utilize these classes

        //small class to tie a player to his ip and port so server can respond and update the player
        struct ConnectedPlayer {

            sf::IpAddress playerIpAddress;
            unsigned short playerPort;

            //id of the last input received from the player that the server has processed
            sf::Uint32 lastConfirmedInputId;

            //past states of this player
            std::tr1::shared_ptr<StateTracker> state;

            UserPlayer player;
        };

    private:

        //server connection manager to send and receive data
        ConnectionManager server;

        std::vector<std::tr1::shared_ptr<ConnectedPlayer> > players;

        //keep track of how long it's been since the last input confirmation packet was sent to the players
        //and all players are updated at the same frequency
        sf::Clock inputConfirmationTime;

        //how often to send update to clients
        sf::Time inputConfirmationDelay;

        //id of the last state update sent to the players so they can check if an old data was sent
        sf::Uint32 lastStateUpdateId;

        //delay for sending state updates to players, state updates contain information about the other players connected
        sf::Clock stateUpdateTimer;
        sf::Time stateUpdateDelay;

        //id of last player created, used to assign a unique id to connecting players by increasing the counter
        int lastPlayerId;

        TeamManager teamManager;

        //receive and handle any incoming data to the server
        void handleIncomingData();

        //handle data sent by the given player
        void handleData(std::tr1::shared_ptr<ConnectedPlayer> player, sf::Packet& data);

        //handle a new input given by the player
        void handlePlayerInput(std::tr1::shared_ptr<ConnectedPlayer> player, sf::Packet& inputPacket);

        //handle a new keystate update from client
        void handlePlayerKeystate(std::tr1::shared_ptr<ConnectedPlayer> player, sf::Packet& statePacket);

        //handle a player's gunfire packet
        void handlePlayerGunfire(std::tr1::shared_ptr<ConnectedPlayer> player, sf::Packet& inputPacket);

        //take the given player's gunshots and handle collision with all other players and objects
        //uses delta fraction and update id to move all other players back in time to check for collision
        void handleGunfireCollision(std::tr1::shared_ptr<ConnectedPlayer> shootingPlayer, const float& deltaFraction, const sf::Uint32& clientUpdateId);

        //handle collision with the bullet and the first player it hits (if it hits any)
        void handleBulletCollision(std::tr1::shared_ptr<ConnectedPlayer> shootingPlayer, std::tr1::shared_ptr<Bullet> bullet, const float& deltaFraction, const sf::Uint32& clientUpdateId);

        //new player connected so create info for him
        void createNewConnection(sf::IpAddress& connectedIp, unsigned short& connectedPort, const int& playerId);

        //send input confirmations to connected clients if it is time to do so
        void sendInputConfirmation();

        //send information about connected clients to other players
        void sendStateUpdates();

        //should be called right after update is given to clients so the state data is saved immediately and will be the same as the data the clients have
        void savePlayerStates(const int stateId);

        //calculate how many updates the server saves before deleting old position data
        int calculateMaxStatesSaved();

        //disconnect the given player, that is, remove him from the container of players
        //take the index of the player in the vector
        void disconnectPlayer(unsigned playerIndex);

        void drawPlayers(sf::RenderWindow& window);

        //check and handle collision between players and flags
        void playerFlagCollision();

    protected:

        //do anything that needs to be done before the game starts running
        //so do all the set up
        //make it virtual so derived classes can modify it to do different things
        virtual void setup(sf::RenderWindow& window);

        virtual void handleWindowEvents(sf::Event& event, sf::RenderWindow& window);

        //handle input from other components, different for each derived class
        virtual void handleComponentInputs(sf::Event& event, sf::RenderWindow& window);

        virtual void updateComponents(sf::RenderWindow& window);

        //updates objects that behave according to delta time
        virtual void updateTimeComponents(const float& delta, sf::RenderWindow& window);

        //handle everything that needs to be called after the update loop is completed
        virtual void handlePostUpdate(sf::RenderWindow& window);

        //draw all components
        virtual void drawComponents(sf::RenderWindow& window);

        virtual void drawMinimap(sf::RenderWindow& window) {}

        virtual const unsigned getFloor() const;

        virtual void handleCollisions();

    public:

        ServerGameManager(unsigned short portToBindTo = 8080);
};

#endif // SERVERGAMEMANAGER_H_INCLUDED
