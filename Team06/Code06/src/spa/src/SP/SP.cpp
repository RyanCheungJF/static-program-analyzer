#include "SP.h"

void SP::processFile(std::string filePath, WritePKB* writePKB, ReadPKB* readPKB) {
    std::ifstream sourceFile(filePath);
    if (!sourceFile) {
        std::cout << "File not found at path: " << filePath << std::endl;
        throw FileNotFoundException("File not found");
    }

    std::stringstream strStream;
    strStream << sourceFile.rdbuf();

    try {
        auto tokens = tokenizer.tokenize(strStream);
        auto root = parser.parseProgram(tokens);
        designExtractor = DesignExtractor(std::move(root), writePKB, readPKB);
        designExtractor.populatePKB();
    } catch (SyntaxErrorException e) {
        std::cout << "Syntax Error caught" << std::endl;
        std::cout << e.what() << std::endl;
        throw e;
    } catch (SemanticErrorException e) {
        std::cout << "Semantic Error caught" << std::endl;
        std::cout << e.what() << std::endl;
        throw e;
    }
}