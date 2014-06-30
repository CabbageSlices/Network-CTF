#include <iostream>
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include <cmath>

using namespace std;

const float PI = 3.14159265;

float degreesToRadians(const float& angleInDegrees) {

    return angleInDegrees * PI / 180.f;
}

float radiansToDegrees(const float& angleInRadians) {

    return angleInRadians * 180.f / PI;
}

int main()
{
    sf::UdpSocket client;

    cout << "hello" << endl;
    sf::Packet packet;
    unsigned short port = 8080;
    sf::IpAddress ip(sf::IpAddress::getLocalAddress());

    while(true) {

        client.send(packet, ip, port);
        sf::sleep(sf::milliseconds(150));
    }

    return 0;
}
