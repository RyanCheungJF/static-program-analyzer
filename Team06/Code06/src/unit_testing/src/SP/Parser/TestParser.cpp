#include "catch.hpp"
#include "../SP/Tokenizer.h"
#include "../SP/Parser.h"
#include "../SP/SPExceptions.h"

TEST_CASE("Valid Source Program") {

}

TEST_CASE("Invalid Source Program") {
	std::stringstream strStream;
	Tokenizer testTokenizer;
	Parser testParser;
	std::deque<Token> tokenQueue;
	std::string testDirectory = "../../../../../../Tests06/sp/parser/";

	SECTION("Program has no procedure") {
		std::ifstream testFile(testDirectory + "invalid1.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}

}