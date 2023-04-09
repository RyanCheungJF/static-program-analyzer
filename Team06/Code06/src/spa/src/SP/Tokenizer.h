#pragma once

#include <deque>
#include <fstream>
#include <regex>
#include <sstream>

#include "../utils/AppConstants.h"
#include "SPExceptions.h"
#include "Token.h"

class Tokenizer {
private:
    static char getNextChar(std::stringstream& file);
    static char peekNextChar(std::stringstream& file);

public:
    static bool isInteger(std::string value);
    std::deque<Token> tokenize(std::stringstream& file);
};