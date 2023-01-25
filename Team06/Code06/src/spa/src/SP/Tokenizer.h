#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

class Tokenizer {

public:
	Tokenizer();

	std::vector<std::string> tokenize(std::stringstream& file);

};

