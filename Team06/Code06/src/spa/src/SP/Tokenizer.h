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
    char getNextChar(std::stringstream& file);
    char peekNextChar(std::stringstream& file);

public:
    bool isInteger(std::string value);
    std::deque<Token> tokenize(std::stringstream& file);
};