#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <deque>
#include <string>
#include <Token.h>

class Tokenizer {

public:
	Tokenizer();

	std::deque<Token> tokenize(std::stringstream& file);

};

