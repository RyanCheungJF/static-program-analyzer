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
    char getNextToken(std::stringstream& file);
    char peekNextToken(std::stringstream& file);

public:
    bool isInteger(std::string value);
    std::deque<Token> tokenize(std::stringstream& file);
};