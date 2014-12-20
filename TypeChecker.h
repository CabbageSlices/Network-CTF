#ifndef TYPECHECKER_H_INCLUDED
#define TYPECHECKER_H_INCLUDED

#include <string>

//checks if all characters in the given string is a digit
//whitespace counts as a non digit character
//empty strings aren't digits either
bool isDigit(std::string toCheck);

#endif // TYPECHECKER_H_INCLUDED
