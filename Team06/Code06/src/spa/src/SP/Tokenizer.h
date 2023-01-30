#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <deque>
#include <string>
#include <regex>
#include <Token.h>
#include <SPExceptions.h>

class Tokenizer {
private:
	bool isInteger(std::string value);
	bool isName(std::string value);

public:
	std::deque<Token> tokenize(std::stringstream& file);
};

