#include "Tokenizer.h"

Tokenizer::Tokenizer() {

}

std::vector<std::string> Tokenizer::tokenize(std::stringstream& file) {
	std::vector<std::string> tokens;
	std::string intermediate;

	while (std::getline(file, intermediate, ' ')) {
		tokens.push_back(intermediate);;
	}

	for (int i = 0; i < tokens.size(); i++) {
		std::cout << tokens[i] << std::endl;
	}

	return tokens;

}