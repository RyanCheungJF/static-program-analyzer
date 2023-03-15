#include "SP.h"

void SP::processFile(std::string filePath, WritePKB* writePKB, ReadPKB* readPKB) {
    std::ifstream sourceFile(filePath);
    std::stringstream strStream;

    if (!sourceFile) {
        std::cerr << "File not found at path: " << filePath << std::endl;
        throw FileNotFoundException("File not found");
    }
    strStream << sourceFile.rdbuf();

    try {
        auto tokens = tokenizer.tokenize(strStream);
        auto root = parser.parseProgram(tokens);
        designExtractor = DesignExtractor(std::move(root), writePKB, readPKB);
        designExtractor.populatePKB();
    } catch (SyntaxErrorException e) {
        std::cerr << "Syntax Error caught" << std::endl;
        std::cerr << e.what() << std::endl;
        throw e;
    } catch (SemanticErrorException e) {
        std::cerr << "Semantic Error caught" << std::endl;
        std::cerr << e.what() << std::endl;
        throw e;
    }
}