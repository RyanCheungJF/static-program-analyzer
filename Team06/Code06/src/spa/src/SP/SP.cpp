#include "SP.h"

void SP::processFile(std::string filePath, WritePKB* writePKB, ReadPKB* readPKB) {
	std::ifstream sourceFile(filePath);
	if (!sourceFile) {
		std::cerr << "File not found" << std::endl;
	}

	std::stringstream strStream;
	strStream << sourceFile.rdbuf();

	try {
		std::deque<Token> tokens = tokenizer.tokenize(strStream);
		std::unique_ptr<Program> root = parser.parseProgram(tokens);
		designExtractor = DesignExtractor(std::move(root), writePKB, readPKB);
		designExtractor.populatePKB();
	} catch (SyntaxErrorException e) {
		std::cout << "Syntax Error caught" << std::endl;
		std::cout << e.what() << std::endl;
	} catch (SemanticErrorException e) {
		std::cout << "Semantic Error caught" << std::endl;
		std::cout << e.what() << std::endl;
	}
}