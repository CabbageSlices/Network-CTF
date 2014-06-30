#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include "SFML/Network.hpp"
#include "ConnectionManager.h"

//a client must connect to the server, send the player's inputs to the server, and receive input confirmation and state updates from the server

class Client : public ConnectionManager {

    private:

        //information regarding the server in order to connect to it
        sf::IpAddress serverIpAddress;
        unsigned short serverPort;

    public:

        //for now give a default server ip and port and leave it hard coded, later have the user type it in
        Client(const sf::IpAddress& ipToConnectTo, const unsigned short& portToConnectTo);

        //attempt to connect to server, timeout if no response is received within the given time limit.
        //return true if connected, false otherwise
        //if connected the id of the client will be given from the server
        bool connectToServer(int& clientId, const sf::Time& responseWaitTime = sf::seconds(2.0));

        //true if succesfully sent, false otherwise
        bool sendToServer(sf::Packet& dataToSend);

        //receives data sent and only downloads it if the packet was sent from the server
        //true if server sent data, false otherwise
        ///warning,if this function receives data from something that isn't the server then the data will be lost
        bool receiveFromServer(sf::Packet& dataDestination);
};

#endif // CLIENT_H_INCLUDED
