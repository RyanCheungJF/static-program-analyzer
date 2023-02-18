#pragma once

#include <sstream>
#include <fstream>
#include <deque>
#include <regex>
#include "Token.h"
#include "SPExceptions.h"
#include "../utils/AppConstants.h"

class Tokenizer {
public:
	bool isInteger(std::string value);
	std::deque<Token> tokenize(std::stringstream& file);
};

