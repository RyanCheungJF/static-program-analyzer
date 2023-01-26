#include "SP.h"

SP::SP() {

}

void SP::processFile(std::string filePath) {
	std::ifstream sourceFile(filePath);

	if (!sourceFile) {
		std::cerr << "File not found" << std::endl;
	}

	std::stringstream strStream;
	strStream << sourceFile.rdbuf();

	std::vector<Token> tokens;
	tokens = tokenizer.tokenize(strStream);

	// parser.parse(tokens);

}