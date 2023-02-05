/*
#include "catch.hpp"
#include "../../../../spa/src/SP/Tokenizer.h"

TEST_CASE("Valid Integer") {

	Tokenizer tokenizer;

	SECTION("Checks that an integer number is in the string") {
		std::string testString = "23";
		REQUIRE(tokenizer.isInteger(testString));
	}
}

TEST_CASE("Invalid Integer") {

	Tokenizer tokenizer;

	SECTION("Checks that leading zero is not a valid integer") {
		std::string testString = "023";
		REQUIRE(!tokenizer.isInteger(testString));
	}

	SECTION("Checks that a mix of alphanumeric is not a valid integer") {
		std::string testString = "1test2";
		REQUIRE(!tokenizer.isInteger(testString));
	}

	SECTION("Checks that a mix of symbols is not a valid integer") {
		std::string testString = "23*4";
		REQUIRE(!tokenizer.isInteger(testString));
	}
}

TEST_CASE("Valid String") {

	Tokenizer tokenizer;

	SECTION("Checks for a valid name with alphabets") {
		std::string testString = "tEsT";
		REQUIRE(tokenizer.isName(testString));
	}

	SECTION("Checks that a mix of alphanumeric is a valid name") {
		std::string testString = "t0e1s2t3";
		REQUIRE(tokenizer.isName(testString));
	}
}

TEST_CASE("Invalid String") {

	Tokenizer tokenizer;

	SECTION("Checks that a valid name has to start with an alphabet") {
		std::string testString = "1abc";
		REQUIRE(!tokenizer.isName(testString));
	}

	SECTION("Checks that a valid name must not contain symbols") {
		std::string testString = "abc&de";
		REQUIRE(!tokenizer.isName(testString));
	}
}
*/