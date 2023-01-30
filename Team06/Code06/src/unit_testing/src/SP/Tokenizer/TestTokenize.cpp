#include "catch.hpp"
#include "../SP/Tokenizer.h"
#include "../SP/SPExceptions.h"
#include <iostream>

TEST_CASE("Valid source program") {
	std::stringstream strStream;
	Tokenizer testTokenizer;
	std::deque<Token> generatedTokens;
	std::deque<Token> expectedTokens;
	std::string testDirectory = "../../../../../../Tests06/sp/";

	SECTION("Valid names") {
		std::ifstream testFile(testDirectory + "valid1.txt");
		strStream << testFile.rdbuf();

		Token t1(TokenType::NAME, "hello");
		Token t2(TokenType::NAME, "There");
		Token t3(TokenType::NAME, "test123");
		Token t4(TokenType::ENDOFFILE, "End of File");
		expectedTokens = {t1, t2, t3, t4};

		generatedTokens = testTokenizer.tokenize(strStream);

		REQUIRE(generatedTokens == expectedTokens);
	}

	SECTION("Valid integers") {
		std::ifstream testFile(testDirectory + "valid2.txt");
		strStream << testFile.rdbuf();

		Token t1(TokenType::INTEGER, "123");
		Token t2(TokenType::INTEGER, "0");
		Token t3(TokenType::INTEGER, "340");
		Token t4(TokenType::ENDOFFILE, "End of File");
		expectedTokens = { t1, t2, t3, t4 };

		generatedTokens = testTokenizer.tokenize(strStream);

		REQUIRE(generatedTokens == expectedTokens);
	}

	SECTION("Valid braces and semicolon") {
		std::ifstream testFile(testDirectory + "valid3.txt");
		strStream << testFile.rdbuf();

		Token t1(TokenType::LEFT_BRACE, "{");
		Token t2(TokenType::RIGHT_BRACE, "}");
		Token t3(TokenType::LEFT_PARENTHESIS, "(");
		Token t4(TokenType::RIGHT_PARENTHESIS, ")");
		Token t5(TokenType::SEMICOLON, ";");
		Token t6(TokenType::ENDOFFILE, "End of File");

		expectedTokens = { t1, t2, t3, t4, t5, t6 };

		generatedTokens = testTokenizer.tokenize(strStream);

		REQUIRE(generatedTokens == expectedTokens);
	}

	SECTION("Valid cond_expr operators") {
		std::ifstream testFile(testDirectory + "valid4.txt");
		strStream << testFile.rdbuf();

		Token t1(TokenType::NOT, "!");
		Token t2(TokenType::AND, "&&");
		Token t3(TokenType::OR, "||");
		Token t4(TokenType::ENDOFFILE, "End of File");

		expectedTokens = { t1, t2, t3, t4 };

		generatedTokens = testTokenizer.tokenize(strStream);

		REQUIRE(generatedTokens == expectedTokens);
	}

}

TEST_CASE("Invalid source program") {
	std::stringstream strStream;
	Tokenizer testTokenizer;
	std::string testDirectory = "../../../../../../Tests06/sp/";

	SECTION("Invalid Integer") {
		std::ifstream testFile(testDirectory + "invalid1.txt");
		strStream << testFile.rdbuf();

		REQUIRE_THROWS_AS(testTokenizer.tokenize(strStream), SyntaxErrorException);
	}

	SECTION("Invalid Integer") {
		std::ifstream testFile(testDirectory + "invalid1.txt");
		strStream << testFile.rdbuf();

		REQUIRE_THROWS_AS(testTokenizer.tokenize(strStream), SyntaxErrorException);
	}

}