#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include "SFML/Graphics.hpp"

#include <string>

//come constants to define when the player closed the input
extern const std::string CLOSED_PROGRAM;
extern const std::string EXITED_FUNCTION;

std::string receiveInput(sf::RenderWindow& window, const sf::Font& font);

#endif // INPUT_H_INCLUDED
