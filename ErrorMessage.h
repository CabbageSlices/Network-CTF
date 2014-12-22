#ifndef ERRORMESSAGE_H_INCLUDED
#define ERRORMESSAGE_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

#include <string>

//an empty screen that displays an error message to the user and he has to close the message to return to whatever screen he was on previously
void displayError(sf::RenderWindow& window, const std::string& message);

#endif // ERRORMESSAGE_H_INCLUDED
