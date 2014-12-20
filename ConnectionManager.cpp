#include "ConnectionManager.h"

ConnectionManager::ConnectionManager(bool isBlocking) :
    communicationSocket()
    {
        communicationSocket.setBlocking(isBlocking);
    }

ConnectionManager::~ConnectionManager() {

    communicationSocket.unbind();
}

bool ConnectionManager::sendData(sf::Packet& packetToSend, const sf::IpAddress& destinationAddress, unsigned short& destinationPort) {

    if(communicationSocket.send(packetToSend, destinationAddress, destinationPort) == sf::Socket::Done) {

        return true;
    }

    return false;
}

bool ConnectionManager::receiveData(sf::Packet& dataDestination, sf::IpAddress& senderAddress, unsigned short& senderPort) {

    if(communicationSocket.receive(dataDestination, senderAddress, senderPort) == sf::Socket::Done && senderAddress.toString() != "") {

        return true;
    }

    return false;
}

bool ConnectionManager::bind(unsigned short portToBindTo) {

    //first unbind to whatever port it was bound to before
    communicationSocket.unbind();

    sf::UdpSocket::Status result = communicationSocket.bind(portToBindTo);

    return result == sf::UdpSocket::Done;
}
