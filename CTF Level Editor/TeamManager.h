#ifndef TEAMMANAGER_H_INCLUDED
#define TEAMMANAGER_H_INCLUDED

#include "SFML/Graphics.hpp"

extern const unsigned short TEAM_A_ID;
extern const unsigned short TEAM_B_ID;

extern const sf::Color TEAM_A_COLOR;
extern const sf::Color TEAM_B_COLOR;

//get the color that corrosponds to the given team
const sf::Color getTeamColor(const unsigned short& team);

#endif // TEAMMANAGER_H_INCLUDED
