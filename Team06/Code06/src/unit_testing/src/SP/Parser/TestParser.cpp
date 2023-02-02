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

		//REQUIRE_THROWS_MATCHES(testParser.parseProgram(tokenQueue), SyntaxErrorException, Catch::Message("SyntaxErrorException::what"));
		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}

	SECTION("Procedure has invalid syntax") {
		std::ifstream testFile(testDirectory + "invalid2.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}

	SECTION("Statement List has no statement") {
		std::ifstream testFile(testDirectory + "invalid3.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}

	SECTION("Invalid read statement") {
		std::ifstream testFile(testDirectory + "invalid4.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}

	SECTION("Invalid print statement") {
		std::ifstream testFile(testDirectory + "invalid5.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}

	SECTION("Invalid call statement") {
		std::ifstream testFile(testDirectory + "invalid6.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}

	SECTION("Invalid while statement") {
		std::ifstream testFile(testDirectory + "invalid7.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}

	SECTION("Invalid if statement") {
		std::ifstream testFile(testDirectory + "invalid8.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}

	SECTION("Invalid assign statement") {
		std::ifstream testFile(testDirectory + "invalid9.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}

	SECTION("Invalid conditional") {
		std::ifstream testFile(testDirectory + "invalid10.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}

	SECTION("Invalid statement type") {
		std::ifstream testFile(testDirectory + "invalid11.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}

	SECTION("Invalid relational expression") {
		std::ifstream testFile(testDirectory + "invalid12.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}

	SECTION("Invalid expression") {
		std::ifstream testFile(testDirectory + "invalid13.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}

	SECTION("Invalid term") {
		std::ifstream testFile(testDirectory + "invalid14.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}

	SECTION("Invalid factor") {
		std::ifstream testFile(testDirectory + "invalid15.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}
}