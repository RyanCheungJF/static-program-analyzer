#include "SP.h"

void SP::processFile(std::string filePath) {
	std::ifstream sourceFile(filePath);

	if (!sourceFile) {
		std::cerr << "File not found" << std::endl;
	}

	std::stringstream strStream;
	strStream << sourceFile.rdbuf();

	std::deque<Token> tokens;
	std::unique_ptr<Program> root;

	try {
		tokens = tokenizer.tokenize(strStream);
		root = parser.parseProgram(tokens);
		designExtractor = DesignExtractor(std::move(root));
		designExtractor.extractEntities();
	} catch (SyntaxErrorException e) {
		std::cout << "Syntax Error caught" << std::endl;
		std::cout << e.what() << std::endl;
	} catch (SemanticErrorException e) {
		std::cout << "Semantic Error caught" << std::endl;
		std::cout << e.what() << std::endl;
	}
}