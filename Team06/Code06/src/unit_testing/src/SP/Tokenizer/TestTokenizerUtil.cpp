#include "catch.hpp"
#include "../../../../spa/src/SP/Tokenizer.h"

TEST_CASE("Checks that an integer number is in the string") {

	Tokenizer tokenizer;
	std::string testString = "23";
	REQUIRE(tokenizer.isInteger(testString));
}

TEST_CASE("Checks that leading zero is not a valid integer") {

	Tokenizer tokenizer;
	std::string testString = "023";
	REQUIRE(!tokenizer.isInteger(testString));
}

TEST_CASE("Checks that a mix of alphanumeric is not a valid integer") {

	Tokenizer tokenizer;
	std::string testString = "1test2";
	REQUIRE(!tokenizer.isInteger(testString));
}

TEST_CASE("Checks that a mix of symbols is not a valid integer") {

	Tokenizer tokenizer;
	std::string testString = "23*4";
	REQUIRE(!tokenizer.isInteger(testString));
}

TEST_CASE("Checks for a valid name with alphabets") {

	Tokenizer tokenizer;
	std::string testString = "tEsT";
	REQUIRE(tokenizer.isName(testString));
}

TEST_CASE("Checks that a mix of alphanumeric is a valid name") {

	Tokenizer tokenizer;
	std::string testString = "t0e1s2t3";
	REQUIRE(tokenizer.isName(testString));
}

TEST_CASE("Checks that a valid name has to start with an alphabet") {

	Tokenizer tokenizer;
	std::string testString = "1abc";
	REQUIRE(!tokenizer.isName(testString));
}

TEST_CASE("Checks that a valid name must not contain symbols") {

	Tokenizer tokenizer;
	std::string testString = "abc&de";
	REQUIRE(!tokenizer.isName(testString));
}