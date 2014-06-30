#ifndef CONNECTIONMANAGER_H_INCLUDED
#define CONNECTIONMANAGER_H_INCLUDED

#include "SFML/Network.hpp"

//class that handles packet exchange, client and server should derive from this

class ConnectionManager {

    private:

        //socket used to cmmunicate with other connected computers
        //default is non blocking and unbound
        sf::UdpSocket communicationSocket;

    public:

        ConnectionManager(bool isBlocking = false);
        ~ConnectionManager();

        //send the given data to the given recipient
        //true if successfuly sent (not if the data was received, just if there was no errors sending)
        bool sendData(sf::Packet& packetToSend, const sf::IpAddress& destinationAddress, unsigned short& destinationPort);

        //receive data and place into given packet, also place the sender information into the given ipAddress and port
        //true if successfully received
        bool receiveData(sf::Packet& dataDestination, sf::IpAddress& senderAddress, unsigned short& senderPort);

        void bind(unsigned short portToBindTo);
};

#endif // CONNECTIONMANAGER_H_INCLUDED
