#include <filesystem>
#include <iostream>

#include "../../spa/src/SP/SP.h"
#include "catch.hpp"

TEST_CASE("Semantically Invalid Source Programs") {
    PKB testPKB;
    WritePKB writePKB;
    ReadPKB readPKB;
    std::stringstream strStream;
    Tokenizer testTokenizer;
    Parser testParser;
    DesignExtractor testDesignExtractor;
    std::deque<Token> tokenQueue;
    std::unique_ptr<Program> root;

    testPKB.initializePkb();
    writePKB.setInstancePKB(testPKB);
    readPKB.setInstancePKB(testPKB);
    std::string errorMessage = "";

    auto testDirectory = std::filesystem::path(UNIT_TESTING_DIR);
    for (int i = 0; i < 3; i++) {
        testDirectory = testDirectory.parent_path();
    }
    testDirectory /= "Tests06/sp/semantics/";

    SECTION("Program with two procedures with same name") {
        std::ifstream testFile(testDirectory.string() + "invalid1.txt");
        strStream << testFile.rdbuf();
        tokenQueue = testTokenizer.tokenize(strStream);
        root = testParser.parseProgram(tokenQueue);
        testDesignExtractor = DesignExtractor(std::move(root), &writePKB, &readPKB);

        try {
            testDesignExtractor.populatePKB();
        } catch (SemanticErrorException e) {
            errorMessage = e.what();
        }

        REQUIRE(errorMessage.find("A program cannot have two procedures with the same name.") != std::string::npos);
    }

    SECTION("Program that calls non-existing procedure") {
        std::ifstream testFile(testDirectory.string() + "invalid2.txt");
        strStream << testFile.rdbuf();
        tokenQueue = testTokenizer.tokenize(strStream);
        root = testParser.parseProgram(tokenQueue);
        testDesignExtractor = DesignExtractor(std::move(root), &writePKB, &readPKB);

        try {
            testDesignExtractor.populatePKB();
        } catch (SemanticErrorException e) {
            errorMessage = e.what();
        }

        REQUIRE(errorMessage.find("A procedure cannot call a non-existing procedure -> nonExistentProcedure") != std::string::npos);
    }

    SECTION("Program with recursive call") {
        std::ifstream testFile(testDirectory.string() + "invalid3.txt");
        strStream << testFile.rdbuf();
        tokenQueue = testTokenizer.tokenize(strStream);
        root = testParser.parseProgram(tokenQueue);
        testDesignExtractor = DesignExtractor(std::move(root), &writePKB, &readPKB);

        try {
            testDesignExtractor.populatePKB();
        } catch (SemanticErrorException e) {
            errorMessage = e.what();
        }

        REQUIRE(errorMessage.find("Recursive and cyclic calls are not allowed") != std::string::npos);
    }

    SECTION("Program with cyclic call") {
        std::ifstream testFile(testDirectory.string() + "invalid4.txt");
        strStream << testFile.rdbuf();
        tokenQueue = testTokenizer.tokenize(strStream);
        root = testParser.parseProgram(tokenQueue);
        testDesignExtractor = DesignExtractor(std::move(root), &writePKB, &readPKB);

        try {
            testDesignExtractor.populatePKB();
        } catch (SemanticErrorException e) {
            errorMessage = e.what();
        }

        REQUIRE(errorMessage.find("Recursive and cyclic calls are not allowed") != std::string::npos);
    }
}