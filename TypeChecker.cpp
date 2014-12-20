#include "TypeChecker.h"
#include <ctype.h>

using std::string;
using std::isdigit;

bool isDigit(string toCheck) {

    for(char ch : toCheck) {

        if(!isdigit(static_cast<int>(ch))) {

            return false;
        }
    }

    return true;
}
