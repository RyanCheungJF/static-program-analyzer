#include "catch.hpp"
#include <filesystem>
#include "SP/Tokenizer.h"
#include "SP/SPExceptions.h"

TEST_CASE("Valid source program") {
	std::stringstream strStream;
	Tokenizer testTokenizer;
	std::deque<Token> generatedTokens;
	std::deque<Token> expectedTokens;
	auto testDirectory = std::filesystem::path(UNIT_TESTING_DIR);
	for (int i = 0; i < 3; i++) {
		testDirectory = testDirectory.parent_path();
	}
	testDirectory /= "Tests06/sp/tokenizer/";

	SECTION("Valid names") {
		std::ifstream testFile(testDirectory.string() + "valid1.txt");
		strStream << testFile.rdbuf();

		Token t1(TokenType::NAME, "hello");
		Token t2(TokenType::NAME, "There");
		Token t3(TokenType::NAME, "test123");
		Token t4(TokenType::ENDOFFILE, "End of File");
		expectedTokens = { t1, t2, t3, t4 };

		generatedTokens = testTokenizer.tokenize(strStream);

		REQUIRE(generatedTokens == expectedTokens);
	}

	SECTION("Valid integers") {
		std::ifstream testFile(testDirectory.string() + "valid2.txt");
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
		std::ifstream testFile(testDirectory.string() + "valid3.txt");
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
		std::ifstream testFile(testDirectory.string() + "valid4.txt");
		strStream << testFile.rdbuf();

		Token t1(TokenType::NOT, "!");
		Token t2(TokenType::AND, "&&");
		Token t3(TokenType::OR, "||");
		Token t4(TokenType::ENDOFFILE, "End of File");

		expectedTokens = { t1, t2, t3, t4 };

		generatedTokens = testTokenizer.tokenize(strStream);

		REQUIRE(generatedTokens == expectedTokens);
	}

	SECTION("Valid comparators") {
		std::ifstream testFile(testDirectory.string() + "valid5.txt");
		strStream << testFile.rdbuf();

		Token t1(TokenType::GREATER, ">");
		Token t2(TokenType::LESS, "<");
		Token t3(TokenType::EQUAL, "==");
		Token t4(TokenType::LESS_EQUAL, "<=");
		Token t5(TokenType::GREATER_EQUAL, ">=");
		Token t6(TokenType::NOT_EQUAL, "!=");
		Token t7(TokenType::ENDOFFILE, "End of File");

		expectedTokens = { t1, t2, t3, t4, t5, t6, t7 };

		generatedTokens = testTokenizer.tokenize(strStream);

		REQUIRE(generatedTokens == expectedTokens);
	}

	SECTION("Valid operators") {
		std::ifstream testFile(testDirectory.string() + "valid6.txt");
		strStream << testFile.rdbuf();

		Token t1(TokenType::MODULO, "%");
		Token t2(TokenType::DIVIDE, "/");
		Token t3(TokenType::MULTIPLY, "*");
		Token t4(TokenType::MINUS, "-");
		Token t5(TokenType::PLUS, "+");
		Token t6(TokenType::ENDOFFILE, "End of File");

		expectedTokens = { t1, t2, t3, t4, t5, t6 };

		generatedTokens = testTokenizer.tokenize(strStream);

		REQUIRE(generatedTokens == expectedTokens);
	}

	// whitespace test for alphanumeric, valid but parser will catch
	SECTION("Valid with no whitespace") {
		std::ifstream testFile(testDirectory.string() + "valid7.txt");
		strStream << testFile.rdbuf();

		Token t1(TokenType::NAME, "aBc12");
		Token t2(TokenType::LEFT_BRACE, "{");
		Token t3(TokenType::RIGHT_BRACE, "}");
		Token t4(TokenType::SEMICOLON, ";");
		Token t5(TokenType::ENDOFFILE, "End of File");

		expectedTokens = { t1, t2, t3, t4, t5 };

		generatedTokens = testTokenizer.tokenize(strStream);

		REQUIRE(generatedTokens == expectedTokens);
	}

	SECTION("Valid sample program with no newlines") {
		std::ifstream testFile(testDirectory.string() + "valid8.txt");
		strStream << testFile.rdbuf();

		Token t1(TokenType::NAME, "procedure");
		Token t2(TokenType::NAME, "A");
		Token t3(TokenType::LEFT_BRACE, "{");
		Token t4(TokenType::NAME, "x");
		Token t5(TokenType::ASSIGN, "=");
		Token t6(TokenType::INTEGER, "1");
		Token t7(TokenType::SEMICOLON, ";");
		Token t8(TokenType::NAME, "y");
		Token t9(TokenType::LESS, "<");
		Token t10(TokenType::NAME, "z");
		Token t11(TokenType::SEMICOLON, ";");
		Token t12(TokenType::RIGHT_BRACE, "}");
		Token t13(TokenType::ENDOFFILE, "End of File");

		expectedTokens = { t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13 };
		generatedTokens = testTokenizer.tokenize(strStream);

		REQUIRE(generatedTokens == expectedTokens);
	}

	SECTION("Valid sample program with lots of whitespaces") {
		std::ifstream testFile(testDirectory.string() + "valid9.txt");
		strStream << testFile.rdbuf();

		Token t1(TokenType::NAME, "procedure");
		Token t2(TokenType::NAME, "A");
		Token t3(TokenType::LEFT_BRACE, "{");
		Token t4(TokenType::NAME, "x");
		Token t5(TokenType::ASSIGN, "=");
		Token t6(TokenType::INTEGER, "1");
		Token t7(TokenType::PLUS, "+");
		Token t8(TokenType::NAME, "y");
		Token t9(TokenType::SEMICOLON, ";");
		Token t10(TokenType::RIGHT_BRACE, "}");
		Token t11(TokenType::ENDOFFILE, "End of File");

		expectedTokens = { t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11 };
		generatedTokens = testTokenizer.tokenize(strStream);

		REQUIRE(generatedTokens == expectedTokens);
	}

	SECTION("Valid sample program with varying whitespaces") {
		std::ifstream testFile(testDirectory.string() + "valid10.txt");
		strStream << testFile.rdbuf();

		Token t1(TokenType::NAME, "procedure");
		Token t2(TokenType::NAME, "A");
		Token t3(TokenType::LEFT_BRACE, "{");
		Token t4(TokenType::NAME, "a");
		Token t5(TokenType::ASSIGN, "=");
		Token t6(TokenType::NAME, "x");
		Token t7(TokenType::PLUS, "+");
		Token t8(TokenType::NAME, "y");
		Token t9(TokenType::SEMICOLON, ";");
		Token t10(TokenType::NAME, "a");
		Token t11(TokenType::ASSIGN, "=");
		Token t12(TokenType::NAME, "x");
		Token t13(TokenType::PLUS, "+");
		Token t14(TokenType::NAME, "y");
		Token t15(TokenType::SEMICOLON, ";");
		Token t16(TokenType::NAME, "a");
		Token t17(TokenType::ASSIGN, "=");
		Token t18(TokenType::NAME, "x");
		Token t19(TokenType::PLUS, "+");
		Token t20(TokenType::NAME, "y");
		Token t21(TokenType::SEMICOLON, ";");
		Token t22(TokenType::RIGHT_BRACE, "}");
		Token t23(TokenType::ENDOFFILE, "End of File");

		expectedTokens = { t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13,
						   t14, t15, t16, t17, t18, t19, t20, t21, t22, t23 };
		generatedTokens = testTokenizer.tokenize(strStream);

		REQUIRE(generatedTokens == expectedTokens);
	}
}

TEST_CASE("Invalid source program") {
	std::stringstream strStream;
	Tokenizer testTokenizer;
	auto testDirectory = std::filesystem::path(UNIT_TESTING_DIR);
	for (int i = 0; i < 3; i++) {
		testDirectory = testDirectory.parent_path();
	}
	testDirectory /= "Tests06/sp/tokenizer/";
	std::string errorMessage = "";

	SECTION("Invalid Integer") {
		std::ifstream testFile(testDirectory.string() + "invalid1.txt");
		strStream << testFile.rdbuf();

		try {
			testTokenizer.tokenize(strStream);
		}
		catch (SyntaxErrorException e) {
			errorMessage = e.what();
		}

		REQUIRE(errorMessage.find("Integer does not follow format") != std::string::npos);
	}

	SECTION("Invalid integer due to no whitespace between alphanumeric characters") {
		std::ifstream testFile(testDirectory.string() + "invalid2.txt");
		strStream << testFile.rdbuf();

		try {
			testTokenizer.tokenize(strStream);
		}
		catch (SyntaxErrorException e) {
			errorMessage = e.what();
		}

		REQUIRE(errorMessage.find("Integer does not follow format") != std::string::npos);
	}

	/*Upon detecting a '&', another '&' must follow(even though no whitespace between
	  non-alphanumeric tokens is allowed), the lone '&' is not a valid token. */
	SECTION("Invalid and operator, lone ampersand") {
		std::ifstream testFile(testDirectory.string() + "invalid3.txt");
		strStream << testFile.rdbuf();

		try {
			testTokenizer.tokenize(strStream);
		}
		catch (SyntaxErrorException e) {
			errorMessage = e.what();
		}

		REQUIRE(errorMessage.find("Expected &&") != std::string::npos);
	}

	SECTION("Invalid or operator, lone |") {
		std::ifstream testFile(testDirectory.string() + "invalid4.txt");
		strStream << testFile.rdbuf();

		try {
			testTokenizer.tokenize(strStream);
		}
		catch (SyntaxErrorException e) {
			errorMessage = e.what();
		}

		REQUIRE(errorMessage.find("Expected ||") != std::string::npos);
	}

	SECTION("Invalid tokens") {
		std::ifstream testFile(testDirectory.string() + "invalid5.txt");
		strStream << testFile.rdbuf();

		try {
			testTokenizer.tokenize(strStream);
		}
		catch (SyntaxErrorException e) {
			errorMessage = e.what();
		}

		REQUIRE(errorMessage.find("Invalid token") != std::string::npos);
	}
}