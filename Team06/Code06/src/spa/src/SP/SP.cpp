#include "SP.h"

void SP::processFile(std::string filePath, WritePKB* writePKB, ReadPKB* readPKB) {
	std::ifstream sourceFile(filePath);
	if (!sourceFile) {
		std::cerr << "File not found" << std::endl;
	}

	std::stringstream strStream;
	strStream << sourceFile.rdbuf();

	try {
		auto tokens = tokenizer.tokenize(strStream);
		auto root = parser.parseProgram(tokens);
		designExtractor = DesignExtractor(std::move(root), writePKB, readPKB);
		designExtractor.populatePKB();
	} catch (SyntaxErrorException e) {
        Exception val = Exception("SyntaxError");
        throw val;
	} catch (SemanticErrorException e) {
        Exception val = Exception("SemanticError");
        throw val;
	}
}