#include "SP.h"

void SP::processFile(std::string filePath) {
	std::ifstream sourceFile(filePath);

	if (!sourceFile) {
		std::cerr << "File not found" << std::endl;
	}

	std::stringstream strStream;
	strStream << sourceFile.rdbuf();

	std::deque<Token> tokens;

	try {
		tokens = tokenizer.tokenize(strStream);
	} catch (SyntaxErrorException e) {
		std::cout << "Syntax Error caught" << std::endl;
		std::cout << e.what() << std::endl;
	} catch (SemanticErrorException e) {
		std::cout << "Semantic Error caught" << std::endl;
		std::cout << e.what() << std::endl;
	}

	std::unique_ptr<Program> program = parser.parseProgram(tokens);
}