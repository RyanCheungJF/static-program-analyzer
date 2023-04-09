#include "SP.h"

void SP::processFile(const std::string& filePath, WritePKB* writePKB, ReadPKB* readPKB) {
    std::ifstream sourceFile(filePath);
    if (!sourceFile) {
        std::cerr << "File not found at path: " << filePath << std::endl;
        throw FileNotFoundException("File not found");
    }

    std::stringstream strStream;
    strStream << sourceFile.rdbuf();

    try {
        auto tokens = tokenizer.tokenize(strStream);
        auto root = parser.parseProgram(tokens);
        designExtractor = DesignExtractor(std::move(root), writePKB, readPKB);
        designExtractor.populatePKB();
    } catch (SyntaxErrorException& e) {
        std::cerr << "Syntax Error caught" << std::endl;
        std::cerr << e.what() << std::endl;
        throw;
    } catch (SemanticErrorException& e) {
        std::cerr << "Semantic Error caught" << std::endl;
        std::cerr << e.what() << std::endl;
        throw;
    }
}