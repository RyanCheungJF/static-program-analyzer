#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <Token.h>

class Tokenizer {

public:
	Tokenizer();

	std::vector<Token> tokenize(std::stringstream& file);

};

