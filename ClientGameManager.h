#ifndef CLIENTGAMEMANAGER_H_INCLUDED
#define CLIENTGAMEMANAGER_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

#include "Client.h"
#include "Player.h"
#include "GameManager.h"

#include <tr1/memory>
#include <vector>

class ClientGameManager: public GameManager {

    private:

        //the current state of the game world determines what kind of actions are handled
        enum GameState {

            STATE_CONNECTING = 0,
            STATE_PLAYING
        };

        GameState currentState;

        Client client;

        //player being controlled by the user
        Player userPlayer;

        //other players connected to world, only interpolates does not have physics
        std::vector<std::tr1::shared_ptr<Player> > connectedPlayers;

        //time how long its been since last state update from the server in order to calculate interpolation fraction
        sf::Clock stateUpdateTimer;

        //lenght of time it took for server to send update, used to calculate interpolation fraction for other clients connected
        sf::Time serverUpdateTime;

        //id of the last update sent by the server to check if its an old update
        sf::Uint32 lastStateUpdateId;

        //interpolate all entities to their new position so drawing looks smooth, the delta fraction is different for each type of entity
        void interpolateEntities();

        //delta fraction for server updates
        const float calculateServerDelta();

        void sendInputsToServer();
        void sendGunshotsToServer();
        void handleServerUpdates();

        void updateUserPlayer(const float& delta, sf::RenderWindow& window);
        void updateConnectedPlayers(const float& delta);

    protected:

        //do anything that needs to be done before the game starts running
        //so do all the set up
        //make it virtual so derived classes can modify it to do different things
        virtual void setup();

        //handle input from other components, different for each derived class
        virtual void handleComponentInputs(sf::Event& event, sf::RenderWindow& window);

        virtual void updateComponents(sf::RenderWindow& window);

        //updates objects that behave according to delta time
        virtual void updateTimeComponents(const float& delta, sf::RenderWindow& window);

        //handle everything that needs to be called after the update loop is completed
        virtual void handlePostUpdate();

        //draw all components
        virtual void drawComponents(sf::RenderWindow& window);


    public:

        ClientGameManager();
};

#endif // GAMEMANAGER_H_INCLUDED
